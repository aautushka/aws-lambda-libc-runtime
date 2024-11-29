This is an ultra-slim, minimalistic C runtime for AWS Lambda.

Features:
- Extremely small deployment package size (~5kb zipped)!
- Under 5ms cold start at smallest allocation size of 128mb
- No dependencies outside of standard C libraries
- No abstraction bloat: HTTP Headers and JSON payload delivered in a raw char* buffer
- No use of malloc in the core: Bring your own arena gc allocator, or use malloc if you want
- Micro-optimized to avoid waste, sometimes without measureable performance gain

This is an alternative to https://github.com/PauloMigAlmeida/aws-lambda-c-runtime

If you are not sure which one to use, you should probably choose the other one. 
