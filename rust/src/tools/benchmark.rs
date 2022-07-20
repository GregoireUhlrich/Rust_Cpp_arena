use super::timer::Timer;

pub fn benchmark(f: &impl Fn()->()) -> f32
{
    let timer = Timer::default();
    f();
    return timer.get_duration_ms();
}

pub fn benchmark_multiple(f: &impl Fn()->(), n_iter: usize) -> Vec<f32>
{
    let mut res = Vec::<f32>::new();
    res.reserve(n_iter);
    for _i in 0..n_iter {
        res.push(benchmark(f));
    }
    return res;
}
