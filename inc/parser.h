#pragma once
#include <memory>
#include <string>

class Plugins;

struct Node {
    virtual ~Node() = default;
    virtual double eval(const Plugins& p) const = 0;
};
using NodePtr = std::unique_ptr<Node>;

class Num : public Node {
public:
    explicit Num(double v) noexcept : value(v) {}
    double eval(const Plugins&) const override { return value; }
private:
    double value;
};

class BinOp : public Node {
public:
    BinOp(char op, NodePtr l, NodePtr r)
        : op(op), left(std::move(l)), right(std::move(r)) {}
    double eval(const Plugins& p) const override;
private:
    char    op;
    NodePtr left, right;
};

class Func : public Node {
public:
    Func(std::string name, NodePtr arg)
        : name(std::move(name)), arg(std::move(arg)) {}
    double eval(const Plugins& p) const override;
private:
    std::string name;
    NodePtr     arg;
};

class Parser {
public:
    explicit Parser(std::string input);
    NodePtr parse();

private:
    std::string input;
    std::size_t pos = 0;

    void skip();
    char peek() const noexcept;
    char take() noexcept;
    bool eat(char c) noexcept;

    NodePtr expr();
    NodePtr term();
    NodePtr power();
    NodePtr unary();
    NodePtr atom();
};