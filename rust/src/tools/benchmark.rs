use super::timer::Timer;

pub fn benchmark(f: &impl Fn()->()) -> u128
{
    let timer = Timer::default();
    f();
    return timer.get_duration_ms();
}

pub fn benchmark_multiple(f: &impl Fn()->(), n_iter: usize) -> Vec<u128>
{
    let mut res = Vec::<u128>::new();
    res.reserve(n_iter);
    for _i in 0..n_iter {
        res.push(benchmark(f));
    }
    return res;
}
