This is an ultra-slim, minimalistic custom runtime for AWS Lambda, with API bindings for C and Rust.

# Features
- No abstraction bloat: HTTP Headers and JSON payload delivered in a raw char* buffer
- No dependencies outside of standard C library. Opt-in to whatever you actually need.
- Dynamically linked to glibc on target al2023 image, to minimize binary size
- No allocator dependency: Bring your own arena or gc allocator, or use malloc if you want
- (Optional) Rust language binding with no_std, no_main and no#tokio
- Rust binary is the same size as C binary (probably because of ELF page-boundary padding).

# Benefits
- Extremely small deployment package size (~5kb zipped)!
- Extremely fast cold starts (4 to 5 milliseconds)
- Minimizes waste of compute resources, slowing down Global Warming!

# Alternatives
- This is an alternative to https://github.com/PauloMigAlmeida/aws-lambda-c-runtime for C
- It's also an alternative to https://github.com/awslabs/aws-lambda-rust-runtime for Rust
- If you are not sure which project to use, you should probably choose one of the others. 
- If you like Javascript, have a look at https://github.com/awslabs/llrt

# Project Structure

- `src/`: The C source code which includes the runtime and an example function
- `tests/`: Unit-integration test using a mock server in Python to emulate the AWS Lambda Environment locally.
- `sam-deploy/`: Scripts for building & deploying using AWS SAM CLI in Docker. This is optional, you could alternatively use CDK, Terraform, the plain AWS CLI, etc.
- `rust-binding/` & `rust-example/`: experimental Rust integration

# Getting Started

> Pre-requisites
```
git clone 
aws configure
```

> C
```
python sam-deploy/deploy.py
```

> Rust
```
python rust-example/build-test-deploy.py
```