This is an ultra-slim, minimalistic C runtime for AWS Lambda, with an optional Rust language binding.

# Features
- Minimize waste of compute resources
- Extremely small deployment package size (~5kb zipped)!
- Under 5ms cold start at smallest allocation size of 128mb
- No dependencies outside of standard C libraries
- No abstraction bloat: HTTP Headers and JSON payload delivered in a raw char* buffer
- No malloc dependency: Bring your own arena or gc allocator, or use malloc if you want
- (Optional) Rust language binding with no_std, no_core and no#tokio overhead

# Alternatives
- This is an alternative to https://github.com/PauloMigAlmeida/aws-lambda-c-runtime
- If you are not sure which project to use, you should probably choose the other one. 
- If you like Javascript, have a look at https://github.com/awslabs/llrt

# Project Structure

- `src/`: The C source code which includes the runtime and an example function
- `tests/`: Unit-integration test using a mock server in Python to emulate the AWS Lambda Environment locally.
- `sam-deploy/`: Scripts for building & deploying using AWS SAM CLI in Docker. This is optional, you could alternatively use CDK, Terraform, the plain AWS CLI, etc.
- `rust-binding/`: experimental Rust integration
