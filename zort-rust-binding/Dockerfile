FROM rust:1.82-alpine
WORKDIR /build
COPY zort-rust-binding/Cargo.toml zort-rust-binding/Cargo.toml
RUN (cd zort-rust-binding && mkdir src && echo 'fn main() {}' > src/lib.rs)
RUN (cd zort-rust-binding && cargo build --release)

COPY . .
RUN (cd zort-rust-binding && cargo build --release)

CMD ["cargo", "test"]