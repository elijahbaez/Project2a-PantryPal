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
    // Splits a CSV line with quotes and double-quote escapes ("")
    static std::vector<std::string> parseCSVLine(const std::string& line) {
        std::vector<std::string> result;
        std::string current = "";
        bool inQuotes = false;

        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (c == '"') {
                // Handle escaped quotes in CSV ("")
                if (i + 1 < line.size() && line[i+1] == '"') {
                    current += '"';
                    i++; // skip second quote
                } else {
                    inQuotes = !inQuotes;
                }
            } else if (c == ',' && !inQuotes) {
                result.push_back(current);
                current = "";
            } else {
                current += c;
            }
        }
        result.push_back(current);
        return result;
    }

    // Parses an array string
    static std::vector<std::string> parseList(const std::string& listStr, bool isIngredient) {
        std::vector<std::string> items;
        std::string current = "";
        bool inQuotes = false;
        char quoteChar = '\0'; // Tracks whether we started with ' or "

        for (size_t i = 0; i < listStr.size(); ++i) {
            char c = listStr[i];

            // Handle escape characters inside the string
            if (c == '\\' && i + 1 < listStr.size()) {
                if (inQuotes) {
                    current += listStr[i + 1];
                }
                i++; // Skip the escaped character so we don't process it twice
                continue;
            }

            if (c == '\'' || c == '"') {
                if (!inQuotes) {
                    // An item is starting
                    inQuotes = true;
                    quoteChar = c;
                } else if (c == quoteChar) {
                    // The matching quote was found, the item is ending
                    if (isIngredient) {
                        std::string sanitized = sanitizeString(current);
                        if (!sanitized.empty()) items.push_back(sanitized);
                    } else {
                        // For steps, don't lowercase it. Trim leading/trailing spaces.
                        size_t start = current.find_first_not_of(" \t\n\r\f\v");
                        if (start != std::string::npos) {
                            current.erase(0, start);
                            current.erase(current.find_last_not_of(" \t\n\r\f\v") + 1);
                            if (!current.empty()) items.push_back(current);
                        }
                    }
                    current = "";
                    inQuotes = false;
                    quoteChar = '\0';
                } else {
                    current += c;
                }
            } else if (inQuotes) {
                current += c;
            }
        }
        return items;
    }

public:
    static std::vector<Recipe> loadData(const std::string& filename) {
        std::vector<Recipe> recipes;
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            std::cerr << "Error: Could not open " << filename << ". Check your working directory!" << std::endl;
            return recipes;
        }

        // Skip header line
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::vector<std::string> cols = parseCSVLine(line);

            // RAW_recipes.csv has 12 columns. Ensure valid row.
            if (cols.size() < 11) continue;

            Recipe r;
            try {
                r.name = cols[0];
                r.id = std::stoi(cols[1]);
                r.minutes = std::stoi(cols[2]);

                // Pass false for steps to preserve formatting/capitalization
                r.steps = parseList(cols[8], false);

                r.description = cols[9];

                // Pass true for ingredients so they get lowercased for the search engine
                r.ingredients = parseList(cols[10], true);

                recipes.push_back(r);
            } catch (const std::exception& e) {
                // Skip rows with malformed numerical data
                continue;
            }
        }

        file.close();
        return recipes;
    }
};