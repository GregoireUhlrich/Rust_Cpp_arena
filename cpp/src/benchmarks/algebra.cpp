#include "algebra.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace bch {

void BaseExpr::print() const
{
    std::cout << to_string() << '\n';
}

std::optional<Expr> BaseExpr::replace(Expr const &, Expr const &) const
{
    return std::nullopt;
};

Number::Number(int val_) : val(val_)
{
}

std::string Number::to_string() const
{
    return std::to_string(val);
}

bool Number::is_equal(Expr const &other) const
{
    OptExpr<Number> converted = other->as<Number>();
    if (converted) {
        return val == converted->val;
    }
    return false;
}

Symbol::Symbol(std::string name_) : name(std::move(name_))
{
}

std::string Symbol::to_string() const
{
    return name;
}

bool Symbol::is_equal(Expr const &other) const
{
    OptExpr<Symbol> converted = other->as<Symbol>();
    if (converted) {
        return name == converted->name;
    }
    return false;
}

std::optional<Expr> Symbol::replace(Expr const &before,
                                    Expr const &after) const
{
    if (is_equal(before)) {
        return after;
    }
    return std::nullopt;
}

Sum::Sum(std::vector<Expr> args_) : args(std::move(args_))
{
}

std::string Sum::to_string() const
{
    std::ostringstream sout;
    for (std::size_t i = 0; i != args.size(); ++i) {
        sout << args[i]->to_string();
        if (i != args.size() - 1) {
            sout << " + ";
        }
    }
    return sout.str();
}

bool Sum::is_equal(Expr const &other) const
{
    OptExpr<Sum> converted = other->as<Sum>();
    if (converted) {
        if (args.size() != converted->args.size()) {
            return false;
        }
        return std::equal(
            args.begin(),
            args.end(),
            converted->args.begin(),
            [](Expr const &A, Expr const &B) { return A->is_equal(B); });
    }
    return false;
}

std::optional<Expr> Sum::replace(Expr const &before, Expr const &after) const
{
    bool              identical = true;
    std::vector<Expr> new_args(args.size());
    std::transform(
        args.begin(), args.end(), new_args.begin(), [&](Expr const &expr) {
            auto opt_res = expr->replace(before, after);
            if (opt_res) {
                identical = false;
            }
            return opt_res.value_or(expr);
        });
    if (!identical) {
        return sum(std::move(new_args));
    }
    return std::nullopt;
}

Expr number(int val)
{
    return std::make_shared<const Number>(val);
}

Expr symbol(std::string name)
{
    return std::make_shared<const Symbol>(std::move(name));
}

Expr sum(std::vector<Expr> args)
{
    return std::make_shared<const Sum>(std::move(args));
}

Expr add(Expr const &a, Expr const &b)
{
    OptExpr<Sum>      a_sum = a->as<Sum>();
    OptExpr<Sum>      b_sum = b->as<Sum>();
    std::vector<Expr> new_args;
    if (a_sum && b_sum) {
        new_args = a_sum->args;
        new_args.insert(
            new_args.end(), b_sum->args.begin(), b_sum->args.end());
    }
    else if (a_sum) {
        new_args = a_sum->args;
        new_args.push_back(b);
    }
    else if (b_sum) {
        new_args = b_sum->args;
        new_args.push_back(a);
    }
    else {
        new_args = {a, b};
    }
    return sum(std::move(new_args));
}

void benchmark_algebra()
{
    Expr a  = number(3);
    Expr b  = number(4);
    Expr x  = symbol("x");
    Expr y  = symbol("y");
    Expr s1 = sum({a, x});
    Expr s2 = sum({a, y});
    s1      = replace(s1, a, b);
    s1      = replace(s1, x, y);
    assert(!a->is_equal(b));
    assert(!x->is_equal(y));
    assert(!s1->is_equal(s2));

    Expr big_sum1 = number(0);
    for (int i = 0; i != 10000; ++i) {
        big_sum1 = add(big_sum1, a);
    }
    Expr big_sum2 = number(0);
    for (int i = 0; i != 10000; ++i) {
        big_sum2 = add(big_sum2, y);
    }
    big_sum2 = replace(replace(big_sum2, y, x), x, a);
    assert(big_sum2->is_equal(big_sum1));
}

} // namespace bch
