#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runtime.h"

slice lambda_handler(const http_recv_buffer *event) {
    // Process the event and return the result as a JSON string
    const char preamble[] = "{\"message\":\"Hello from C! Event received: ";
    int count_quotes = 0;
    for (char* p = event->body.data; *p; ++p) {
        if (*p == '"') {
            ++count_quotes;
        }
    }
    slice result;
    result.len = sizeof(preamble) + event->body.len + count_quotes + 1;
    result.data = malloc(result.len);

    strcpy(result.data, preamble);
    char* p = result.data + sizeof(preamble) - 1;
    for (char* q = event->body.data; *q; ++q) {
        if (*q == '"') {
            *p++ = '\\';
        }
        *p++ = *q;
    }
    strcpy(p, "\"}"); // +2 from here, -1 from sizeof(preamble) == +1 above
    return result;
}

void cleanup(slice *result) {
    free(result->data);
    result->data = NULL;
}

int main() {
    DEBUG("Main started\n");
    return start_lambda(lambda_handler, cleanup);
}