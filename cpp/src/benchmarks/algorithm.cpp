#include "algorithm.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_map>

namespace bch {

std::vector<Point> Board::get_neighboors(Point const &p) const
{
    std::vector<Point> neighboors = {
        {p.i + 1, p.j},
        {p.i - 1, p.j},
        {p.i, p.j + 1},
        {p.i, p.j - 1},
    };
    auto last = std::remove_if(
        begin(neighboors), end(neighboors), [&](Point const &p) {
            return p.i >= this->size || p.j >= this->size;
        });
    neighboors.erase(last, neighboors.end());
    return neighboors;
}

void walk(Board const                                   board,
          std::vector<Point>                           &points,
          std::unordered_map<Point, bool, Point::hash> &previous)
{
    std::vector<Point> next;
    next.reserve(4 * points.size());
    for (const auto &p : points) {
        auto neighboors = board.get_neighboors(p);
        for (const auto &n : neighboors) {
            auto pos = previous.find(n);
            if (pos == previous.end()) {
                previous[n] = true;
                next.push_back(n);
            }
        }
    }
    points = std::move(next);
}

bool reached_end_game(Board const &board, std::vector<Point> const &points)
{
    return std::any_of(begin(points), end(points), [&](Point const &p) {
        return board.get(p);
    });
}

void benchmark_algorithm()
{
    constexpr std::size_t SIZE = 1000;
    Board                 board{SIZE};
    board.set({0, 0}, true);
    std::unordered_map<Point, bool, Point ::hash> previous;
    std::vector<Point> points = {Point{SIZE - 1, SIZE - 1}};
    std::size_t        count  = 0;
    while (!reached_end_game(board, points) && !points.empty()) {
        walk(board, points, previous);
        ++count;
    }
    assert(count == 2 * (SIZE - 1));
}

} // namespace bch
