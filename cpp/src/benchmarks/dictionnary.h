#ifndef BCH_DICTIONNAR_H_INCLUDED
#define BCH_DICTIONNAR_H_INCLUDED

#include <map>
#include <string>
#include <string_view>

namespace bch {

struct Node {

    bool contains(std::string_view::const_iterator first,
                  std::string_view::const_iterator last);

    void record(std::string_view::const_iterator first,
                std::string_view::const_iterator last);

    bool                 is_terminal = false;
    std::map<char, Node> children;
};

class Dict {

  public:
    void record(std::string_view str);

    void print();

    bool contains(std::string_view word);

  private:
    static void print_node(Node const &node, std::string str);

  private:
    Node root;
};

void benchmark_dictionnary();

} // namespace bch

#endif
