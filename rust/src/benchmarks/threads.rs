use std::sync::{Mutex, Arc};
use std::thread;

pub fn benchmark_threads() -> ()
{
    const N_THREADS: u64 = 10;
    const N_ITER: u64 = 1_000_000;
    let counter = Arc::new(Mutex::new(0 as u64));
    let mut threads = Vec::new();
    for _i in 0..N_THREADS {
        let counter = counter.clone();
        threads.push(thread::spawn(move || {
            for _j in 0..N_ITER {
                let mut data = counter.lock().unwrap();
                *data += 1;
            }
        }));
    }
    for thread in threads {
        thread.join().unwrap();
    }
    assert!(*counter.lock().unwrap() == N_THREADS*N_ITER);
}
