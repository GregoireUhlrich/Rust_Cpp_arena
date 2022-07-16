#include <iostream>
#include <optional>
#include <algorithm>
#include <string>

using std::cout, std::endl, std::cerr, std::cin, std::string;

struct Point {
    int i, j;
};

Point &operator+=(Point &A, Point const &B)
{
    A.i += B.i;
    A.j += B.j;
    return A;
}

Point operator+(Point const &A, Point const &B)
{
    Point res = A;
    res += B;
    return res;
}

void print_board(const char board[9])
{
    cout << "-------\n";
    for (int i = 0; i != 3; ++i) {
        cout << '|' << board[3*i] << '|' << board[3*i+1] << '|' << board[3*i+2] << '|';
        cout << "\n-------\n";
    }
}

std::optional<Point> is_winning_pos(
        const char   board[9],
        char         player,
        Point        start,
        Point const &delta
        )
{
    std::optional<Point> winning;
    for (int i = 0; i != 3; ++i) {
        char val = board[3*start.i + start.j];
        if (val == ' ') {
            if (winning)  {
                return std::nullopt;
            }
            winning = start;
        }
        else if (val != player) {
            return std::nullopt;
        }
        start += delta;
    }
    return winning;
}

std::optional<int> winning_pos(const char board[9], char player)
{
    std::pair<Point, Point> rows[] = {
        {Point{0, 0}, Point{1, 0}},
        {Point{0, 0}, Point{0, 1}},
        {Point{0, 0}, Point{1, 1}},
        {Point{0, 1}, Point{1, 0}},
        {Point{0, 2}, Point{1, 0}},
        {Point{1, 0}, Point{0, 1}},
        {Point{2, 0}, Point{0, 1}},
        {Point{2, 0}, Point{-1, 1}}
    };
    for (const auto &[start, delta] : rows) {
        auto pos = is_winning_pos(board, player, start, delta);
        if (pos) {
            return 3*pos->i + pos->j;
        }
    }
    return std::nullopt;
}

int first_empty(const char board[9])
{
    for (int i = 0; i != 9; ++i) {
        if (board[i] == ' ')
            return i;
    }
    return -1;
}

void play_ia(char board[9])
{
    auto win = winning_pos(board, 'x');
    if (win) {
        board[win.value()] = 'x';
    }
    else {
        auto lose = winning_pos(board, 'o');
        if (lose) {
            board[lose.value()] = 'x';
        }
        else {
            board[first_empty(board)] = 'x';
        }
    }
}

void play_human(char board[9])
{
    string buffer;
    unsigned long pos;
    do {
        cout << "Enter player position in [0, 9[: ";
        cin >> buffer;
        try {
            pos = std::stoul(buffer);
        }
        catch (std::exception const &err) {
            cerr << "The answer \"" << buffer << "\" could not be interpreted:\n";
            cerr << err.what();
            continue;
        }
    } while (pos >= 9 || board[pos] != ' ');
    board[pos] = 'o';
}

bool end_game(const char board[9])
{
    if (std::count(board, board+9, ' ') == 0) {
        return true;
    }
    return false;
}

void game(char board[9], bool human_first = true)
{
    bool end = end_game(board);
    if (end) {
        cout << "Game has ended!\n";
        return;
    }
    if (human_first) {
        play_human(board);
    }
    else {
        play_ia(board);
    }
    print_board(board);
    game(board, !human_first);
}

int main() {

    char board[9];
    std::fill(board, board+9, ' ');
    print_board(board);
    game(board);

    return 0;
}
