FROM rust:1.82-alpine
WORKDIR /build
COPY rust-binding/Cargo.toml rust-binding/Cargo.toml
RUN (cd rust-binding && mkdir src && echo 'fn main() {}' > src/lib.rs)
RUN (cd rust-binding && cargo build --release)

COPY . .
RUN (cd rust-binding && cargo build --release)

CMD ["cargo", "test"]