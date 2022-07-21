use super::super::tools::dynamic::dynamic;

pub fn benchmark_array() -> ()
{
    const N: u64 = 1000;
    let mut arr: [[u64; N as usize]; N as usize] = [[0; N as usize]; N as usize];
    for i in 0..arr.len() {
        for j in 0..arr.len() {
            arr[i][j] = (i+j) as u64;
        }
    }
    let mut sum: u64 = 0;
    let rand = dynamic();
    for i in 0..arr.len() {
        for j in 0..arr.len() {
            if i as i32 != rand {
                sum += arr[i][j];
            }
        }
    }
    assert!(sum == N*N*(N-1) - N*rand as u64 - (N*(N-1))/2);
}
