#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN {
public:
    RPN();
    RPN(const RPN&);
    RPN& operator=(const RPN&);
    ~RPN();

    // Evaluate tokens like: "8 9 * 9 - 9 - 9 - 4 - 1 +"
    // Tokens must be single-digit numbers (0..9) or + - * /
    long evaluate(const std::string& expr) const;

private:
    static bool isOperator(char c);
    static bool isDigitToken(const std::string& t);
};

#endif
