#include <algorithm>
#include <optional>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>


class BaseExpr;
class Number;
class Symbol;
class Sum;

using Expr = std::shared_ptr<const BaseExpr>;

Expr sum(std::vector<Expr> args);
Expr symbol(std::string name);
Expr number(int val);

template <typename T>
using OptExpr = T const*;

class BaseExpr {
public:

    virtual ~BaseExpr() {}

    virtual std::string to_string() const = 0;

    virtual bool is_equal(Expr const &other) const = 0;

    void print() const {
        std::cout << to_string() << '\n';
    }

    virtual std::optional<Expr> replace(
                Expr const &, 
                Expr const &
            ) const 
    {
        return std::nullopt;
    };

    template<typename T>
        OptExpr<T> as() const {
            return dynamic_cast<T const *>(this);
        }
};

Expr replace(Expr const &init, Expr const &before, Expr const &after)
{
    return init->replace(before, after).value_or(init);
}

class Number: public BaseExpr {

public:

    Number(int val_ = 0): val(val_) {}

    std::string to_string() const override {
        return std::to_string(val);
    }

    bool is_equal(Expr const &other) const override {
        OptExpr<Number> converted = other->as<Number>();
        if (converted) {
            return val == converted->val;
        }
        return false;
    }

private:

    int val;
};

class Symbol: public BaseExpr {

public:

    Symbol(std::string name_): name(std::move(name_)) {}

    std::string to_string() const override {
        return name;
    }

    bool is_equal(Expr const &other) const override {
        OptExpr<Symbol> converted = other->as<Symbol>();
        if (converted) {
            return name == converted->name;
        }
        return false;
    }

    std::optional<Expr> replace(
            Expr const &before, 
            Expr const &after
            ) const override
    {
        if (is_equal(before)) {
            return after;
        }
        return std::nullopt;
    }

private:

    std::string name;
};

class Sum: public BaseExpr {

public:

    Sum(std::vector<Expr> args_): args(std::move(args_)) {}

    std::string to_string() const override {
        std::ostringstream sout;
        for (std::size_t i = 0; i != args.size(); ++i) {
            sout << args[i]->to_string();
            if (i != args.size() - 1) {
                sout << " + ";
            }
        }
        return sout.str();
    }

    bool is_equal(Expr const &other) const override {
        OptExpr<Sum> converted = other->as<Sum>();
        if (converted) {
            if (args.size() != converted->args.size()) {
                return false;
            }
            return std::equal(args.begin(), args.end(), converted->args.begin(),
                    [](Expr const &A, Expr const &B) { return A->is_equal(B); });
        }
        return false;
    }

    std::optional<Expr> replace(
            Expr const &before, 
            Expr const &after
            ) const override
    {
        bool identical = true;
        std::vector<Expr> new_args(args.size());
        std::transform(
                args.begin(), 
                args.end(), 
                new_args.begin(),
                [&](Expr const &expr) { 
                    auto opt_res = expr->replace(before, after); 
                    if (opt_res) {
                        identical = false;
                    }
                    return opt_res.value_or(expr);
                }
        );
        if (!identical) {
            return sum(std::move(new_args));
        }
        return std::nullopt;
    }

private:

    std::vector<Expr> args;
};

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

int main() {

    Expr a = number(3);
    Expr b = number(4);
    Expr x = symbol("x");
    Expr y = symbol("y");
    Expr s1 = sum({a, x});
    Expr s2 = sum({a, y});
    a->print();
    b->print();
    x->print();
    y->print();
    s1->print();
    s2->print();
    std::cout << a->is_equal(a) << std::endl;
    std::cout << a->is_equal(b) << std::endl;
    std::cout << x->is_equal(a) << std::endl;
    std::cout << x->is_equal(x) << std::endl;
    std::cout << x->is_equal(y) << std::endl;
    std::cout << s1->is_equal(x) << std::endl;
    std::cout << s1->is_equal(s1) << std::endl;
    std::cout << s1->is_equal(s2) << std::endl;
    s1 = replace(s1, a, b);
    s1 = replace(s1, x, y);
    s1->print();
    s2->print();
    std::cout << s1->is_equal(s1) << std::endl;
    std::cout << s1->is_equal(s2) << std::endl;
    
    Expr big_sum1 = sum(std::vector<Expr>(1000, x));
    Expr big_sum2 = sum(std::vector<Expr>(1000, big_sum1));
    Expr big_sum3 = sum(std::vector<Expr>(1000, a));
    Expr big_sum4 = sum(std::vector<Expr>(1000, big_sum3));
    big_sum2 = replace(replace(big_sum2, x, y), y, a);
    std::cout << big_sum2->is_equal(big_sum4) << std::endl;


    return 0;
}
