use std::path::PathBuf;
use project_root::get_project_root;

pub fn get_results_path() -> PathBuf
{
    let mut path = get_project_root().unwrap();
    path.push("results");
    path
}

pub fn get_data_path() -> PathBuf
{
    let mut path = get_project_root().unwrap();
    path.push("data");
    path
}
