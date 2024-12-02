#![no_std]
#![no_main]

use aws_lambda_libc::api::{lambda_event_loop, Event, Writer};

#[no_mangle]
#[link_section = ".text._start"]
pub extern "C" fn _start() -> ! {
    lambda_event_loop(handler);
}

fn handler(event: &Event, writer: &mut Writer) -> () {
    // Process the event and return the result as a JSON string
    writer.write_str("{\"message\":\"Hello from Rust! Event received: ");
    // loop over event body and write it to the writer, escaping quotes
    for byte in event.body {
        if *byte == b'"' {
            writer.write_str("\\\"");
        } else {
            writer.write_str(core::str::from_utf8(&[*byte]).unwrap());
        };
    }
    writer.write_str("\"}");
}
