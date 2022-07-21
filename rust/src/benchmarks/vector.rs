use std::vec::Vec;

#[derive(Clone)]
struct Object {
    _id: i32,
    _version: String,
    _partners: Vec::<String>
}

impl Default for Object {
    fn default() -> Object {
        Object {
            _id: 0,
            _version: "Some version".to_string(),
            _partners: vec![
                "A".to_string(),
                "BB".to_string(),
                "CCC".to_string(),
                "DDDD".to_string(),
                "ZZZZZZZZZZZZZZZZZZZZZZZZZ".to_string()
                ]
        }
    }
}

pub fn benchmark_vector1() -> ()
{
    let vec1 = vec![Object::default(); 500000];
    let vec2 = vec1.clone();
    assert!(vec1.len() == vec2.len());
}

pub fn benchmark_vector2() -> ()
{
    let mut vec1 = vec![];
    for _i in 0..500000 {
        vec1.push(Object::default());
    }
    assert!(vec1.len() == 500000);
}

pub fn benchmark_vector3() -> ()
{
    let mut vec1 = vec![];
    vec1.reserve(500000);
    for _i in 0..500000 {
        vec1.push(Object::default());
    }
    assert!(vec1.len() == 500000);
}
