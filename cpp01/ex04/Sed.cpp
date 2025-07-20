#include "Sed.hpp"
#include <fstream>
#include <iostream>

Sed::Sed(const std::string &filename) {
    std::ifstream in(filename.c_str());
    if (!in) {
        std::cerr << "Error: cannot open " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(in, line)) {
        _content += line;
        if (!in.eof())
            _content += '\n';
    }
    in.close();
}

Sed::~Sed() {}

void Sed::replace(const std::string &s1, const std::string &s2) {
    if (s1.empty()) return;
    std::string result;
    size_t pos = 0;
    while (true) {
        size_t idx = _content.find(s1, pos);
        if (idx == std::string::npos) {
            result += _content.substr(pos);
            break;
        }
        result += _content.substr(pos, idx - pos) + s2;
        pos = idx + s1.length();
    }
    _content.swap(result);
}

void Sed::writeTo(const std::string &outname) const {
    std::ofstream out(outname.c_str());
    if (!out) {
        std::cerr << "Error: cannot open " << outname << std::endl;
        return;
    }
    out << _content;
    out.close();
}
