#include <iostream>
#include <vector>
#include <chrono>
#include "Recipe.h"
#include "DataParser.h"

int main() {
    std::cout << "--- PantryPal Recipe Finder ---" << std::endl;
    std::cout << "Loading dataset..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Recipe> allRecipes = DataParser::loadData("RAW_recipes.csv");
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Successfully loaded " << allRecipes.size() << " recipes.\n" << std::endl;
    return 0;
}