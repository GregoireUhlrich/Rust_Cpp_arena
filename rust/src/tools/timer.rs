use std::time::Instant;

pub struct Timer {
    start: Instant
}

impl Default for Timer {
    fn default() -> Timer {
        Timer {start: Instant::now()}
    }
}
impl Timer {

    pub fn get_duration_ms(&self) -> u128 {
        let now = Instant::now();
        return (now - self.start).as_millis();
    }
}
