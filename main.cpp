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
    
    auto mapStart = std::chrono::high_resolution_clock::now();
    HashMap ingredientMap;
    for (size_t i = 0; i < allRecipes.size(); ++i) {
        for (const auto& ingredient : allRecipes[i].ingredients) {
            ingredientMap.insert(ingredient, i);
        }
    }
    auto mapStop = std::chrono::high_resolution_clock::now();
    auto mapDuration = std::chrono::duration_cast<std::chrono::milliseconds>(mapStop - mapStart);
    std::cout << "Custom Hash Map built: " << mapDuration.count() << " ms" << std::endl;
    return 0;
}