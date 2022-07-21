#ifndef BCH_MEMOIZATION_H_INCLUDED
#define BCH_MEMOIZATION_H_INCLUDED

#include <algorithm>
#include <vector>

namespace bch {

struct Point {
    std::size_t i;
    std::size_t j;

    struct hash {
        std::size_t operator()(Point const &p) const noexcept
        {
            return std::hash<std::size_t>{}(p.i)
                   ^ std::hash<std::size_t>{}(p.j << 16);
        }
    };

    bool operator==(Point const &p) const
    {
        return i == p.i && j == p.j;
    }
};

class Board {
  public:
    Board(std::size_t size_) : size(size_), board(size * size)
    {
    }

    bool get(Point const &p) const
    {
        return board[index(p.i, p.j)];
    }

    void set(Point const &p, bool value)
    {
        board[index(p.i, p.j)] = value;
    }

    std::vector<Point> get_neighboors(Point const &p) const;

  private:
    std::size_t index(std::size_t i, std::size_t j) const
    {
        return i * this->size + j;
    }

  private:
    std::size_t       size;
    std::vector<bool> board;
};

void benchmark_memoization();

} // namespace bch

#endif
