#![no_std]
#![no_main]

use zort_rust_binding::api::{start_lambda_listener, Event, Writer};
use core::panic::PanicInfo;

#[panic_handler]
pub fn panic(_info: &PanicInfo) -> ! {
    unsafe { core::arch::asm!("int3"); }
    loop {}
}

fn handler(event: &Event, writer: &mut Writer) -> () {
    // Process the event and return the result as a JSON string
    writer.write_str("{\"message\":\"Hello from C! Event received: ");
    // loop over event body and write it to the writer, escaping quotes
    for byte in event.body {
        let result = if *byte == b'"' {
            writer.write_str("\\\"");
        } else {
            writer.write_str(core::str::from_utf8(&[*byte]).unwrap());
        };
    }
    writer.write_str("\"}");
}

#[no_mangle]
#[link_section = ".text._start"]
pub extern "C" fn _start() -> ! {
    unsafe {
        core::arch::asm!(
            "and rsp, -16",  // Align stack to 16 bytes
            options(nomem, nostack)
        );
    }
    start_lambda_listener(handler);
    unsafe { core::arch::asm!("int3"); }
    loop {}
}
