#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runtime.h"

int lambda_handler(const http_recv_buffer *event, char *output_buffer) {
    // Process the event and return the result as a JSON string
    const char preamble[] = "{\"message\":\"Hello from C! Event received: ";
    
    strcpy(output_buffer, preamble);

    char* p = output_buffer + sizeof(preamble) - 1;
    for (char* q = event->body.data; *q; ++q) {
        if (*q == '"') {
            *p++ = '\\';
        }
        *p++ = *q;
    }
    strcpy(p, "\"}");
    return strlen(output_buffer);
}

void main() {
    DEBUG("Main started\n");
    start_lambda(lambda_handler);
}