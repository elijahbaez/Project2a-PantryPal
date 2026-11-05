#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <iterator>
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

    if (allRecipes.empty()) {
        std::cout << "Failed to load dataset. Exiting...\n";
        return 1;
    }

    std::cout << "Successfully loaded " << allRecipes.size() << " recipes.\n" << std::endl;

    // --- Build Data Structures ---
    HashMap ingredientMap;
    RedBlackTree ingredientTree;

    auto mapStart = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < allRecipes.size(); ++i) {
        for (const auto& ingredient : allRecipes[i].ingredients) {
            ingredientMap.insert(ingredient, i); // Storing vector index for O(1) lookup
        }
    }
    auto mapStop = std::chrono::high_resolution_clock::now();
    auto mapDuration = std::chrono::duration_cast<std::chrono::milliseconds>(mapStop - mapStart);
    std::cout << "Custom Hash Map built: " << mapDuration.count() << " ms" << std::endl;

    auto treeStart = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < allRecipes.size(); ++i) {
        for (const auto& ingredient : allRecipes[i].ingredients) {
            ingredientTree.insert(ingredient, i);
        }
    }
    auto treeStop = std::chrono::high_resolution_clock::now();
    auto treeDuration = std::chrono::duration_cast<std::chrono::milliseconds>(treeStop - treeStart);
    std::cout << "Red-Black Tree built: " << treeDuration.count() << " ms\n" << std::endl;

    // --- CLI MENU ---
    while (true) {
        std::cout << "Enter your ingredients (comma separated) or 'Q' to quit:\n> ";
        std::string userInput;
        std::getline(std::cin, userInput);

        if (userInput == "Q" || userInput == "q") break;
        if (userInput.empty()) continue;

        std::vector<std::string> searchTerms = splitIngredients(userInput);
        if (searchTerms.empty()) continue;

        // Map Search & Intersection
        auto searchMapStart = std::chrono::high_resolution_clock::now();
        std::vector<int> mapResults = ingredientMap.search(searchTerms[0]);
        for (size_t i = 1; i < searchTerms.size(); ++i) {
            mapResults = intersectVectors(mapResults, ingredientMap.search(searchTerms[i]));
        }
        auto searchMapStop = std::chrono::high_resolution_clock::now();
        auto mapSearchDur = std::chrono::duration_cast<std::chrono::milliseconds>(searchMapStop - searchMapStart);

        // Tree Search & Intersection
        auto searchTreeStart = std::chrono::high_resolution_clock::now();
        std::vector<int> treeResults = ingredientTree.search(searchTerms[0]);
        for (size_t i = 1; i < searchTerms.size(); ++i) {
            treeResults = intersectVectors(treeResults, ingredientTree.search(searchTerms[i]));
        }
        auto searchTreeStop = std::chrono::high_resolution_clock::now();
        auto treeSearchDur = std::chrono::duration_cast<std::chrono::milliseconds>(searchTreeStop - searchTreeStart);

        std::cout << "\nSearch time (Map): " << mapSearchDur.count() << " ms" << std::endl;
        std::cout << "Search time (Tree): " << treeSearchDur.count() << " ms\n" << std::endl;

        if (mapResults.empty()) {
            std::cout << "No recipes found with all those ingredients. Try again!\n\n";
            continue;
        }

        // --- RESULTS LOOP ---
        while (true) {
            std::cout << "--- Results (Top Matches) ---" << std::endl;
            int displayCount = std::min(10, (int)mapResults.size());
            for (int i = 0; i < displayCount; ++i) {
                Recipe& r = allRecipes[mapResults[i]];
                std::cout << i + 1 << ". " << r.name << " (Prep: " << r.minutes << " mins)" << std::endl;
            }

            std::cout << "\nEnter a recipe number to view details, 'S' to search again, or 'Q' to quit.\n> ";
            std::string subCommand;
            std::getline(std::cin, subCommand);

            if (subCommand == "Q" || subCommand == "q") {
                return 0;
            } else if (subCommand == "S" || subCommand == "s") {
                std::cout << "\n";
                break;
            } else {
                try {
                    int choice = std::stoi(subCommand);
                    if (choice >= 1 && choice <= displayCount) {
                        Recipe& selected = allRecipes[mapResults[choice - 1]];
                        std::cout << "\n--- Recipe Details: " << selected.name << " ---" << std::endl;
                        std::cout << "Prep Time: " << selected.minutes << " mins\n";
                        std::cout << "Description: " << selected.description << "\n";

                        std::cout << "Ingredients: ";
                        for (size_t i = 0; i < selected.ingredients.size(); ++i) {
                            std::cout << selected.ingredients[i] << (i < selected.ingredients.size() - 1 ? ", " : "");
                        }
                        std::cout << "\nSteps:\n";
                        for (size_t i = 0; i < selected.steps.size(); ++i) {
                            std::cout << i + 1 << ". " << selected.steps[i] << "\n";
                        }

                        std::cout << "\nPress Enter to go back to results...";
                        std::getline(std::cin, subCommand);
                        std::cout << "\n";
                    } else {
                        std::cout << "Invalid number. Try again.\n\n";
                    }
                } catch (...) {
                    std::cout << "Invalid input. Try again.\n\n";
                }
            }
        }
    }

    return 0;
}