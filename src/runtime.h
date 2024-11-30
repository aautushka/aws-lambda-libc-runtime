
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
    char *data;
    size_t len;
} slice;

typedef struct {
    slice buffer;
    slice awsRequestId;
    slice body;
} http_recv_buffer;

// higher-level interface ("bring your own handler"):

int start_lambda(slice (*handler)(const http_recv_buffer*), void (*cleanup)(slice *));

// lower-level interface ("bring your own loop") - simplifies access via Rust FFI:

typedef struct runtime runtime;

runtime* runtime_init();

http_recv_buffer* get_next_request(const runtime *rt);

void send_response(const runtime *rt, const char *response, size_t response_len);
