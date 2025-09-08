#include "RPN.hpp"
#include <sstream>
#include <stdexcept>
#include <cctype>

RPN::RPN() {}
RPN::RPN(const RPN&) {}
RPN& RPN::operator=(const RPN&) { return *this; }
RPN::~RPN() {}

bool RPN::isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

bool RPN::isDigitToken(const std::string& t) {
    return (t.size() == 1 && std::isdigit(static_cast<unsigned char>(t[0])));
}

long RPN::evaluate(const std::string& expr) const {
    std::stack<long> st;
    std::istringstream iss(expr);
    std::string tok;

    while (iss >> tok) {
        if (isDigitToken(tok)) {
            long v = tok[0] - '0'; // 0..9
            st.push(v);
        } else if (tok.size() == 1 && isOperator(tok[0])) {
            if (st.size() < 2) throw std::runtime_error("bad");
            long b = st.top(); st.pop();
            long a = st.top(); st.pop();
            long r;
            switch (tok[0]) {
                case '+': r = a + b; break;
                case '-': r = a - b; break;
                case '*': r = a * b; break;
                case '/':
                    if (b == 0) throw std::runtime_error("div0");
                    r = a / b; // integer division
                    break;
                default: throw std::runtime_error("op");
            }
            st.push(r);
        } else {
            // invalid token (multi-digit, brackets, decimals, etc.)
            throw std::runtime_error("tok");
        }
    }
    if (st.size() != 1) throw std::runtime_error("rest");
    return st.top();
}
