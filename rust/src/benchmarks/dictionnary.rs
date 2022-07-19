use std::collections::BTreeMap;
use std::str::Chars;
use super::super::tools::paths::get_data_path;

#[derive(Default, Clone)]
pub struct Node {
    pub is_terminal: bool,
    children: BTreeMap<char, Node>
}

#[derive(Default, Clone)]
pub struct Dict {
    pub root: Node
}

impl Node {
    pub fn record(&mut self, word: &mut Chars)
    {
        let c: char;
        match word.next() {
            Some(ch) => c = ch,
            None => {
                self.is_terminal = true;
                return;
            }
        };
        let node = self.children.entry(c).or_insert(Node::default());
        node.record(word);
    }

    pub fn contains(&self, word: &mut Chars) -> bool
    {
        match word.next() {
            Some(ch) => {
                let next = self.children.get(&ch);
                if next.is_some() {
                    return next.unwrap().contains(word);
                }
                else {
                    return false;
                }
            },
            None => return self.is_terminal
        }
    }
}

impl Dict {
    pub fn record(&mut self, str: &str)
    {
        self.root.record(&mut str.chars());
    }

    pub fn contains(&self, str: &str) -> bool
    {
        self.root.contains(&mut str.chars())
    }

    // fn print_node(node: &Node, str: String)
    // {
    //     if node.is_terminal {
    //         println!(" --> {}", &str);
    //     }
    //     for (ch, next) in &node.children {
    //         let mut str_cpy = str.clone();
    //         str_cpy.push(*ch);
    //         Dict::print_node(&next, str_cpy);
    //     }
    // }

    // pub fn print(&self)
    // {
    //     println!("Dictionnary content:");
    //     Dict::print_node(&self.root, "".to_string());
    // }
}

use std::io::{BufRead, BufReader};
use std::fs::File;

pub fn benchmark_dictionnary() -> ()
{
    let mut dict = Dict::default();
    let mut path = get_data_path();
    path.push("dict.txt");
    let reader = BufReader::new(File::open(&path).expect("Cannot open dict.txt"));

    for line in reader.lines() {
        dict.record(&line.unwrap());
    }
    assert!(dict.contains("black-hole"));
    assert!(!dict.contains("white-hole"));
}
