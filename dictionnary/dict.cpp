#include <iostream>
#include <fstream>
#include <map>
#include <string>

struct Node {

    void record(
            std::string_view::const_iterator first, 
            std::string_view::const_iterator last)
    {
        if (first == last) {
            is_terminal = true;
            return;
        }
        Node &next = children[*first];
        next.record(first+1, last);
    }

    bool is_terminal = false;
    std::map<char, Node> children;
};

class Dict {
    
public:

    void record(std::string_view str)
    {
        root.record(str.begin(), str.end());
    }

    void print()
    {
        std::cout << "Dictionnary content:\n";
        print_node(root, "");
        std::cout.flush();
    }

private:

    static 
    void print_node(Node const &node, std::string str)
    {
        if (node.is_terminal) {
            std::cout << "  --> " << str << '\n';
        }
        for (const auto &[ch, next] : node.children) {
            print_node(next, str + ch);
        }
    }

private:
    Node root;
};

int main() {

    Dict dict;
    std::ifstream in("dict.txt");
    if (!in) {
        std::cerr << "File \"dict.txt\" is invalid!" << std::endl;
        return 1;
    }
    std::string buffer;
    while (!in.eof()) {
        in >> buffer;
        dict.record(buffer);
    }
    //dict.print();
    return 0;
}
