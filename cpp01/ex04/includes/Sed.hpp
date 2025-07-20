#ifndef SED_HPP
#define SED_HPP

#include <string>

class Sed {
private:
    std::string _content;
public:
    Sed(const std::string &filename);
    ~Sed();
    void replace(const std::string &s1, const std::string &s2);
    void writeTo(const std::string &outname) const;
};

#endif  // SED_HPP
