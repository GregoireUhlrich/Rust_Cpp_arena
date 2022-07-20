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
    for i in 0..arr.len() {
        for j in 0..arr.len() {
            sum += arr[i][j];
        }
    }
    assert!(sum == N*N*(N-1));
}
