#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include "Recipe.h"

class DataParser {
private:
    static std::vector<std::string> parseCSVLine(const std::string& line) {
        std::vector<std::string> result;
        std::string current = "";
        bool inQuotes = false;
        for (char c : line) {
            if (c == '"') inQuotes = !inQuotes;
            else if (c == ',' && !inQuotes) { result.push_back(current); current = ""; }
            else current += c;
        }
        result.push_back(current);
        return result;
    }
    static std::vector<std::string> parseList(const std::string& listStr, bool isIngredient) {
        std::vector<std::string> items;
        std::string current = "";
        bool inQuotes = false;
        for (char c : listStr) {
            if (c == '\'' || c == '"') {
                if (inQuotes) {
                    if (isIngredient) {
                        std::string s = sanitizeString(current);
                        if (!s.empty()) items.push_back(s);
                    } else { items.push_back(current); }
                    current = "";
                }
                inQuotes = !inQuotes;
            } else if (inQuotes) current += c;
        }
        return items;
    }
public:
    static std::vector<Recipe> loadData(const std::string& filename) {
        std::vector<Recipe> recipes;
        std::ifstream file(filename);
        std::string line;
        if (!file.is_open()) return recipes;
        std::getline(file, line);
        while (std::getline(file, line)) {
            std::vector<std::string> cols = parseCSVLine(line);
            if (cols.size() < 11) continue;
            Recipe r;
            try {
                r.name = cols[0]; r.id = std::stoi(cols[1]); r.minutes = std::stoi(cols[2]);
                r.steps = parseList(cols[8], false); r.description = cols[9];
                r.ingredients = parseList(cols[10], true);
                recipes.push_back(r);
            } catch (...) { continue; }
        }
        return recipes;
    }
};