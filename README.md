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

Below is a comparison of AWS Lambda custom runtimes, focusing on cold start latency:

| AWS Lambda Runtime | Cold Start | Link |
|---------|------------|---------|
| This Project | under 5ms | https://github.com/refacktor-aws/aws-lambda-libc |
| Paul Almeida's C | n/a | https://github.com/PauloMigAlmeida/aws-lambda-c-runtime |
| Official Rust | 40-80ms | https://github.com/awslabs/aws-lambda-rust-runtime |
| LLRT (JavaScript) | 48-85ms | https://github.com/awslabs/llrt |
| Nim | 10-40ms | https://github.com/refacktor-aws/nim-aws-sam |
| Zig | 11ms | https://github.com/by-nir/aws-lambda-zig |
| Java/Graal AOTC | 110ms | https://github.com/refacktor-aws/java-native-aws-lambda-without-frameworks |
| Plain JIT Java | 400ms | n/a |
| All the rest | ~200ms+ | The python and nodejs runtimes are slower than they should be. Go is a little faster, C# is a little slower. |

# Project Structure

- `src/`: The C source code for the AWS Custom Runtime Interface implementation
- `tests/`: Unit-integration test using a mock server in Python to emulate the AWS Lambda Environment locally. Includes function.c, an example lambda function in C.
- `sam-deploy/`: Scripts for building & deploying using AWS SAM CLI in Docker. This is optional, you could alternatively use CDK, Terraform, the plain AWS CLI, etc.
- `rust-binding/` & `rust-example/`: experimental Rust integration

# Getting Started

*Pre-requisites*

- `git clone` (this project) 
- `docker` container cli (e.g. Rancher Desktop, for building & local testing)
- `aws configure` (for deployment)
- `python` to run the build scripts

*C*

- `python sam-deploy/deploy.py`

*Rust*
- `python rust-example/build-test-deploy.py`
