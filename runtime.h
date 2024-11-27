
#define LOG(...) fprintf(stderr, __VA_ARGS__)

#ifndef RELEASE
#define DEBUG(...) fprintf(stderr, __VA_ARGS__)
#define FATAL(COND, MSG) { if(COND) { LOG(MSG); exit(-1); } }
#else
#define DEBUG(...)
#define FATAL(COND, MSG)
#endif

#define MAX_BODY_SIZE 6 * 1048576
#define MAX_HEADER_SIZE 8 * 1024
#define MAX_BUFFER_SIZE (MAX_BODY_SIZE + MAX_HEADER_SIZE)

typedef struct {
    char *body;
    char *awsRequestId;
    char *buffer;
} http_buffer;

int start_lambda(char* (*handler)(const http_buffer*), void (*cleanup)(char*));
