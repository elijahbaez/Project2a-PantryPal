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
    static std::vector<std::string> parseCSVLine(const std::string& line) { return {}; }
    static std::vector<std::string> parseList(const std::string& listStr, bool isIngredient) { return {}; }
public:
    static std::vector<Recipe> loadData(const std::string& filename) { return {}; }
};