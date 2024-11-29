#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runtime.h"

char* lambda_handler(const http_buffer *event) {
    // Process the event and return the result as a JSON string
    const char preamble[] = "{\"message\":\"Hello from C! Event received: ";
    int count_quotes = 0;
    for (char* p = event->body; *p; ++p) {
        if (*p == '"') {
            ++count_quotes;
        }
    }
    char* result = malloc(sizeof(preamble) + strlen(event->body) + count_quotes + 1);
    strcpy(result, preamble);
    char* p = result + strlen(result);
    for (char* q = event->body; *q; ++q) {
        if (*q == '"') {
            *p++ = '\\';
        }
        *p++ = *q;
    }
    strcpy(p, "\"}"); // +2 from here, -1 from sizeof(preamble) == +1 above
    return result;
}

void cleanup(char *result) {
    free(result);
}

int main() {
    DEBUG("Main started\n");
    return start_lambda(lambda_handler, cleanup);
}