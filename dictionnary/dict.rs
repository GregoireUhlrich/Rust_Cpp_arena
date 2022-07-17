pub mod dict {

    use std::collections::BTreeMap;
    use std::str::Chars;

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
    }

    impl Dict {
        pub fn record(&mut self, str: &str)
        {
            self.root.record(&mut str.chars());
        }

        fn print_node(node: &Node, str: String)
        {
            if node.is_terminal {
                println!(" --> {}", &str);
            }
            for (ch, next) in &node.children {
                let mut str_cpy = str.clone();
                str_cpy.push(*ch);
                Dict::print_node(next, str_cpy);
            }
        }

        pub fn print(&self)
        {
            println!("Dictionnary content:");
            Dict::print_node(&self.root, "".to_string());
        }
    }
}

fn main() {
    let mut dict = dict::Dict::default();
    dict.record("Hello");
    dict.record("Hulk");
    dict.record("Hat-trick");
    dict.record("World");
    dict.record("Wonderful");
    dict.record("Rust");
    dict.print();
}
