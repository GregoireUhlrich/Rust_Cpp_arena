pub fn benchmark_algorithm() -> ()
{
    const N: usize = 100000;
    let elements: Vec<i32> = vec![0; N];
    let mut n: i32 = 0;
    let res = elements.iter()
        .map(|_| {
            let res = n;
            n += 3;
            res
        })
        .filter(|x| x.abs() > 10)
        .map(|x| (-x))
        .max();
    assert!(res.unwrap() == -12);
}
