#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/mman.h>

#include "runtime.h"

char *startswith(char *s, const char *prefix) {
    while (*prefix) {
        if (*prefix != *s) {
            return NULL;
        }
        ++prefix;
        ++s;
    }
    while (*s == ' ') {
        ++s;
    }
    return s;
}

struct addrinfo *resolve_host(const char *host_and_port) {
    struct addrinfo hints;
    struct addrinfo *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;    // Use IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_protocol = 0;        // Any protocol

    char *colon = strchr(host_and_port, ':');
    char host_no_port[strlen(host_and_port)+1];
    char *port;
    if(colon != NULL) {
        strncpy(host_no_port, host_and_port, colon - host_and_port);
        host_no_port[colon - host_and_port] = '\0';
        port = colon + 1;
    } else {
        strcpy(host_no_port, host_and_port);
        port = "80";
    }

    int rc = getaddrinfo(host_no_port, port, &hints, &result);
    DEBUG("getaddrinfo[%s:%s] returned rc=%d\n", host_no_port, port, rc);
    FATAL(rc != 0, "getaddrinfo failed");

    return result;
}

int send_all(int sockfd, const char *buf, int len) {
    int total_sent = 0;
    while (total_sent < len) {
        int rc = send(sockfd, buf + total_sent, len - total_sent, 0);
        FATAL(rc < 0, "Failed to send data\n");
        total_sent += rc;
        DEBUG("Sent %d bytes, total sent=%d\n", rc, total_sent);
    }
    return total_sent;
}

int socket_connect(const struct addrinfo *addr) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    FATAL(sockfd < 0, "Failed to create socket\n");

    // Set socket timeout
    struct timeval timeout;      
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int rc = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    FATAL(rc < 0, "Failed to set socket timeout\n");

    rc = connect(sockfd, addr->ai_addr, addr->ai_addrlen);

    FATAL(rc < 0, "Connection failed\n");
    DEBUG("Connected\n");
    return sockfd;
}

void make_http_request(const struct addrinfo *addr, const char *host, const char* path, const char* method, char *body, http_buffer *hb) {
    DEBUG("Making HTTP request to host=[%s], path=[%s], method=[%s]\n", host, path, method);
    int sockfd = socket_connect(addr);

    int len = body ? strlen(body) : 0;
    char request[MAX_HEADER_SIZE];
    snprintf(request, sizeof(request),
            "%s %s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\n"
            "Content-Length: %ld\r\n\r\n",
            method, path, host, len);

    DEBUG("Request headers:\n<<<\n%s\n>>>\n", request);
    send_all(sockfd, request, strlen(request));
    
    if (len > 0) {
        send_all(sockfd, body, len);
    }

    char *response = hb->buffer;
    char *parse_point = response;
    char *line_start = response;
    char *body_start = NULL;
    int total_bytes_received = 0;
    int content_length = -1;
    int remain = MAX_BUFFER_SIZE;

    while(remain) {
        
        FATAL(parse_point >= response + MAX_BUFFER_SIZE, "Buffer overflow");

        if (parse_point >= response + total_bytes_received) {
            DEBUG("Calling recv with ptr=%d, remain=%d\n", total_bytes_received, remain);
            int bytes_received = recv(sockfd, response + total_bytes_received, remain, 0);
            DEBUG("Received %d bytes\n", bytes_received);
            FATAL(bytes_received <= 0, "Failed to receive bytes\n");
            total_bytes_received += bytes_received;
            remain -= bytes_received;
            if (body_start != NULL) {
                parse_point += bytes_received;
                continue;
            }
        }
        if (*parse_point == '\n') {
            char *value_ptr;
            FATAL(parse_point - response < 3, "Unexpected linebreak before HTTP headers");
            FATAL(parse_point[-1] != '\r', "Malformed linebreak: no \\r before \\n");
            if (parse_point[-2] == '\n') {
                FATAL(content_length < 0, "Missing Content-Length header.");
                body_start = parse_point + 1;
                remain = content_length - ((response + total_bytes_received) - body_start);
                DEBUG("BODY START: %p, remain=%d\n", body_start, remain);
                parse_point = response + total_bytes_received;
                continue;
            }
            else
            if((value_ptr = startswith(line_start, "Content-Length:"))) {
                content_length = atoi(value_ptr);
                DEBUG("HEADER Content-Length: %d\n", content_length);
            }
            else 
            if((value_ptr = startswith(line_start, "Lambda-Runtime-Aws-Request-Id:"))) {
                hb->awsRequestId = value_ptr;
                while(*value_ptr != '\r') {
                    value_ptr++;
                }
                value_ptr[0] = '\0';
                DEBUG("HEADER Lambda-Runtime-Aws-Request-Id: [%s]\n", hb->awsRequestId);
            }
            else
            if((value_ptr = startswith(line_start, "HTTP/1.0 410"))) {
                DEBUG("410 (shutting down)\n");
                exit(0);
            }
            line_start = parse_point + 1;
        }

        ++parse_point;
    }
    DEBUG("Total bytes received: %d\n", total_bytes_received);
    response[total_bytes_received] = '\0';
    hb->body = body_start;
    close(sockfd);
    DEBUG("Response received\n");
}

int start_lambda(char* (*handler)(const http_buffer*), void (*cleanup)(char*)) {
    const char* runtimeApi = getenv("AWS_LAMBDA_RUNTIME_API");
    FATAL(runtimeApi == NULL, "AWS_LAMBDA_RUNTIME_API environment variable not set\n");
    DEBUG("Runtime API: %s\n", runtimeApi);
    struct addrinfo *addrinfo = resolve_host(runtimeApi);

    char requestUrl[256];
    snprintf(requestUrl, sizeof(requestUrl), "http://%s/2018-06-01/runtime/invocation/next", runtimeApi);

    http_buffer hb;
    hb.buffer = mmap(NULL, 
        MAX_BUFFER_SIZE, 
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    char responseUrl[256];

    while (1) {

        make_http_request(addrinfo, runtimeApi, requestUrl, "GET", "", &hb);
        FATAL(hb.awsRequestId == NULL, "Missing Lambda-Runtime-Aws-Request-Id header\n");

        char* lambda_response = handler(&hb);

        snprintf(responseUrl, sizeof(responseUrl), "http://%s/2018-06-01/runtime/invocation/%s/response", runtimeApi, hb.awsRequestId);

        hb.awsRequestId = NULL;
        hb.body = NULL;
        make_http_request(addrinfo, runtimeApi, responseUrl, "POST", lambda_response, &hb);

        cleanup(lambda_response);
    }
    return 0;
}
