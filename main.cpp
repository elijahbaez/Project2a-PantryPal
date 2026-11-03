#include <iostream>
#include <vector>
#include <chrono>
#include "Recipe.h"
#include "DataParser.h"
#include "RedBlackTree.h"
#include "HashMap.h"

// Helper to split comma separated input
std::vector<std::string> splitIngredients(const std::string& input) {
    std::vector<std::string> ingredients;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, ',')) {
        std::string cleaned = sanitizeString(item);
        if (!cleaned.empty()) {
            ingredients.push_back(cleaned);
        }
    }
    return ingredients;
}

// Helper to intersect two vectors (finding common recipe indices)
std::vector<int> intersectVectors(std::vector<int> v1, std::vector<int> v2) {
    std::vector<int> result;
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(result));
    return result;
}

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

    auto treeStart = std::chrono::high_resolution_clock::now();
    RedBlackTree ingredientTree;
    for (size_t i = 0; i < allRecipes.size(); ++i) {
        for (const auto& ingredient : allRecipes[i].ingredients) {
            ingredientTree.insert(ingredient, i);
        }
    }
    auto treeStop = std::chrono::high_resolution_clock::now();
    auto treeDuration = std::chrono::duration_cast<std::chrono::milliseconds>(treeStop - treeStart);
    std::cout << "Red-Black Tree built: " << treeDuration.count() << " ms\n" << std::endl;

    return 0;
}