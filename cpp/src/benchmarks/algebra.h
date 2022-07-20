#ifndef BCH_ALGEBRA_H_INCLUDED
#define BCH_ALGEBRA_H_INCLUDED

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace bch {

class BaseExpr;
class Number;
class Symbol;
class Sum;

using Expr = std::shared_ptr<const BaseExpr>;

Expr sum(std::vector<Expr> args);
Expr symbol(std::string name);
Expr number(int val);

template <typename T>
using OptExpr = T const *;

class BaseExpr {
  public:
    virtual ~BaseExpr()
    {
    }

    virtual std::string to_string() const = 0;

    virtual bool is_equal(Expr const &other) const = 0;

    void print() const;

    virtual std::optional<Expr> replace(Expr const &, Expr const &) const;

    template <typename T>
    OptExpr<T> as() const
    {
        return dynamic_cast<T const *>(this);
    }
};

inline Expr replace(Expr const &init, Expr const &before, Expr const &after)
{
    return init->replace(before, after).value_or(init);
}

class Number : public BaseExpr {

  public:
    Number(int val_ = 0);

    std::string to_string() const override;

    bool is_equal(Expr const &other) const override;

  public:
    int val;
};

class Symbol : public BaseExpr {

  public:
    Symbol(std::string name_);

    std::string to_string() const override;

    bool is_equal(Expr const &other) const override;

    std::optional<Expr> replace(Expr const &before,
                                Expr const &after) const override;

  public:
    std::string name;
};

class Sum : public BaseExpr {

  public:
    Sum(std::vector<Expr> args_);

    std::string to_string() const override;

    bool is_equal(Expr const &other) const override;

    std::optional<Expr> replace(Expr const &before,
                                Expr const &after) const override;

  public:
    std::vector<Expr> args;
};

Expr sum(std::vector<Expr> args);
Expr symbol(std::string name);
Expr number(int val);

Expr add(Expr const &a, Expr const &b);

void benchmark_algebra();

} // namespace bch

#endif
