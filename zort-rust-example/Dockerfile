FROM amazonlinux:2023 AS builder
RUN yum install -y gcc-11.4.1-2.amzn2023.0.2

ARG TARGET=x86_64-unknown-linux-gnu

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | \
    sh -s -- -y --default-toolchain 1.83.0 && \
    . "$HOME/.cargo/env" && \
    rustup target add ${TARGET}

ENV PATH="/root/.cargo/bin:${PATH}"

WORKDIR /build/zort-rust-binding
COPY zort-rust-binding/Cargo.toml .
RUN mkdir src && echo 'fn main() {}' > src/lib.rs
RUN cargo build --target ${TARGET} --release

WORKDIR /build
COPY . .

WORKDIR /build/zort-rust-binding
RUN cargo build --target ${TARGET} --release

WORKDIR /build/zort-rust-example
RUN cargo build --target ${TARGET} --release
RUN ln -f target/${TARGET}/release/bootstrap /build/bootstrap

#
# Local integration test using mock server
#
FROM amazonlinux:2023 AS tester
RUN yum install -y python3 python3-pip
RUN pip3 install requests
WORKDIR /test
COPY --from=builder /build/bootstrap /test/bootstrap
COPY tests/test_runtime.py .
#RUN python3 test_runtime.py /test/bootstrap
CMD ["python3", "test_runtime.py", "/test/bootstrap"]
#CMD ["sh"]
