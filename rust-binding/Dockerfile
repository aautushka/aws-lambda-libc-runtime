#
# simple "canary build" to check if the crate compiles at all
#

FROM rust:1.82-alpine

WORKDIR /build/rust-binding
COPY Cargo.toml ./
RUN mkdir src && echo 'fn main() {}' > src/lib.rs
RUN cargo build

WORKDIR /build
COPY . .

WORKDIR /build/rust-binding
RUN cargo build
