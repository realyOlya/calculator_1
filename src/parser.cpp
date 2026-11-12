#include "parser.h"
#include "plugins.h"
#include <cctype>
#include <cmath>
#include <stdexcept>

double BinOp::eval(const Plugins& p) const {
    const double a = left->eval(p);
    const double b = right->eval(p);
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0.0) throw std::runtime_error("division by zero");
        return a / b;
    case '^': return std::pow(a, b);
    default:  throw std::runtime_error("bad operator");
    }
}

double Func::eval(const Plugins& p) const {
    const double x = arg->eval(p);
    const std::optional<double> r = p.call(name, x);
    if (!r) throw std::runtime_error("unknown function: " + name);
    return *r;
}

Parser::Parser(std::string input) : input(std::move(input)) {}

void Parser::skip() {
    while (pos < input.size() && std::isspace(static_cast<unsigned char>(input[pos])))
        ++pos;
}

char Parser::peek() const noexcept {
    return pos < input.size() ? input[pos] : '\0';
}

char Parser::take() noexcept {
    skip();
    return pos < input.size() ? input[pos++] : '\0';
}

bool Parser::eat(char c) noexcept {
    skip();
    if (peek() == c) { ++pos; return true; }
    return false;
}

NodePtr Parser::parse() {
    NodePtr n = expr();
    if (pos != input.size())
        throw std::runtime_error("unexpected characters at end");
    return n;
}

NodePtr Parser::expr() {
    NodePtr l = term();
    while (true) {
        const char op = peek();
        if (op == '+' || op == '-') {
            take();
            NodePtr r = term();
            l = std::make_unique<BinOp>(op, std::move(l), std::move(r));
        }
        else break;
    }
    return l;
}

NodePtr Parser::term() {
    NodePtr l = power();
    while (true) {
        const char op = peek();
        if (op == '*' || op == '/') {
            take();
            NodePtr r = power();
            l = std::make_unique<BinOp>(op, std::move(l), std::move(r));
        }
        else break;
    }
    return l;
}

NodePtr Parser::power() {
    NodePtr l = unary();
    if (eat('^')) {
        NodePtr r = power();
        return std::make_unique<BinOp>('^', std::move(l), std::move(r));
    }
    return l;
}

NodePtr Parser::unary() {
    if (eat('-'))
        return std::make_unique<BinOp>('-', std::make_unique<Num>(0.0), unary());
    if (eat('+')) return unary();
    return atom();
}

NodePtr Parser::atom() {
    if (eat('(')) {
        NodePtr n = expr();
        if (!eat(')')) throw std::runtime_error("expected ')'");
        return n;
    }

    if (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '.') {
        const std::size_t start = pos;
        while (pos < input.size() && (std::isdigit(static_cast<unsigned char>(input[pos])) || input[pos] == '.'))
            ++pos;
        return std::make_unique<Num>(std::stod(input.substr(start, pos - start)));
    }

    if (std::isalpha(static_cast<unsigned char>(peek()))) {
        const std::size_t start = pos;
        while (pos < input.size() && std::isalnum(static_cast<unsigned char>(input[pos])))
            ++pos;
        std::string name = input.substr(start, pos - start);

        if (!eat('(')) throw std::runtime_error("expected '(' after function");
        NodePtr a = expr();
        if (!eat(')')) throw std::runtime_error("expected ')' after argument");
        return std::make_unique<Func>(std::move(name), std::move(a));
    }

    throw std::runtime_error("unexpected token");
}