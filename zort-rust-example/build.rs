// build.rs
fn main() {
    println!("cargo:rustc-link-lib=dylib=c");
    println!("cargo:rustc-link-arg=-nostartfiles");
}