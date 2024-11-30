use core::slice::from_raw_parts;
use crate::bindings::{runtime_init, get_next_request, send_response};

pub struct Event<'a> {
    pub body: &'a [u8]
}

pub struct Writer {
    buffer: [u8; 64000],
    position: usize,
}

impl Writer {
    pub fn write_str(&mut self, s: &str) -> () {
        let bytes = s.as_bytes();
        if self.position + bytes.len() > self.buffer.len() {
            panic!("Buffer overflow");
        }
        
        self.buffer[self.position..self.position + bytes.len()]
            .copy_from_slice(bytes);
        self.position += bytes.len();
    }
}

pub fn start_lambda_listener(handler_fn: impl Fn(&Event, &mut Writer) -> ()) -> () {
    unsafe {
        let runtime = runtime_init();
        loop {
            let request = get_next_request(runtime);
            let event = Event {
                body: from_raw_parts((*request).body.data, (*request).body.data_len)
            };
            let mut writer = Writer {
                buffer: [0; 64000],
                position: 0,
            };
            handler_fn(&event, &mut writer);
            send_response(runtime, writer.buffer.as_ptr(), writer.position);
        }
    }
}