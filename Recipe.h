#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

struct Recipe {
    int id;
    std::string name;
    int minutes;
    std::string description;
    std::vector<std::string> ingredients;
    std::vector<std::string> steps;
};

inline std::string sanitizeString(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
    const char* t = " \t\n\r\f\v\"\'[]";
    size_t start = s.find_first_not_of(t);
    if (start == std::string::npos) return ""; 
    s.erase(0, start);
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}