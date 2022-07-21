use std::env;

fn may_fail(i: i32) -> Result<u32, &'static str>
{
    if i < env::args().len() as i32 {
        Err("Oups you gave a negative number!")
    }
    else {
        Ok(i as u32)
    }
}

fn may_fail2(i: i32) -> Option<u32>
{
    if i < env::args().len() as i32 {
        None
    }
    else {
        Some(i as u32)
    }
}

pub fn benchmark_error() -> ()
{
    const N: i32 = 10_000;
    let mut sum: u32 = 0;
    for i in -N..N {
        let res = may_fail(i);
        sum += match res {
            Ok(value) => value ,
            Err(_) => 0
        };
    }
    let n = env::args().len() as u32;
    assert!(sum == 5_000*9_999 - (n*(n-1))/2);
}

pub fn benchmark_option() -> ()
{
    const N: i32 = 10_000;
    let mut sum: u32 = 0;
    for i in -N..N {
        let res = may_fail2(i);
        sum += match res {
            Some(value) => value ,
            None => 0
        };
    }
    let n = env::args().len() as u32;
    assert!(sum == 5_000*9_999 - (n*(n-1))/2);
}
