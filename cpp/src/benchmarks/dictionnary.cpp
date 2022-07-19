#include "dictionnary.h"
#include "tools/error.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <string_view>

namespace bch {

void Node::record(std::string_view::const_iterator first,
                  std::string_view::const_iterator last)
{
    if (first == last) {
        is_terminal = true;
        return;
    }
    Node &next = children[*first];
    next.record(first + 1, last);
}
void Dict::record(std::string_view str)
{
    root.record(str.begin(), str.end());
}

void Dict::print()
{
    std::cout << "Dictionnary content:\n";
    print_node(root, "");
    std::cout.flush();
}

void Dict::print_node(Node const &node, std::string str)
{
    if (node.is_terminal) {
        std::cout << "  --> " << str << '\n';
    }
    for (const auto &[ch, next] : node.children) {
        print_node(next, str + ch);
    }
}

void benchmark_dictionnary()
{
    Dict          dict;
    std::ifstream in(DATA_DIR "/dict.txt");
    if (!in) {
        throw IOError("File \"dict.txt\" not found!");
    }
    std::string buffer;
    while (!in.eof()) {
        in >> buffer;
        dict.record(buffer);
    }
}

} // namespace bch
