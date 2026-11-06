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
                if (i + 1 < line.size() && line[i+1] == '"') {
                    current += '"';
                    i++;
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
        char quoteChar = '\0';

        for (size_t i = 0; i < listStr.size(); ++i) {
            char c = listStr[i];

            // Only escape quotes and slashes.
            if (c == '\\' && i + 1 < listStr.size()) {
                char nextChar = listStr[i+1];
                if (nextChar == '\'' || nextChar == '"' || nextChar == '\\') {
                    if (inQuotes) current += nextChar;
                    i++;
                    continue;
                }
            }

            if (c == '\'' || c == '"') {
                if (!inQuotes) {
                    inQuotes = true;
                    quoteChar = c;
                } else if (c == quoteChar) {
                    if (isIngredient) {
                        std::string sanitized = sanitizeString(current);
                        if (!sanitized.empty()) items.push_back(sanitized);
                    } else {
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

        std::getline(file, line); // Skip header

        std::string fullRow = "";
        bool inQuotes = false;

        // MULTI-LINE CSV FIX
        while (std::getline(file, line)) {
            if (fullRow.empty()) {
                fullRow = line;
            } else {
                fullRow += "\n" + line; // Re-add the newline that getline stripped
            }

            // Check if this line closes the open quotes
            for (size_t i = 0; i < line.size(); ++i) {
                if (line[i] == '"') {
                    // Handle escaped quotes ""
                    if (i + 1 < line.size() && line[i+1] == '"') {
                        i++;
                    } else {
                        inQuotes = !inQuotes;
                    }
                }
            }

            if (inQuotes) {
                continue;
            }

            std::vector<std::string> cols = parseCSVLine(fullRow);
            fullRow = ""; // Reset

            if (cols.size() < 11) continue;

            Recipe r;
            try {
                r.name = cols[0];
                r.id = std::stoi(cols[1]);
                r.minutes = std::stoi(cols[2]);
                r.steps = parseList(cols[8], false);
                r.description = cols[9];
                r.ingredients = parseList(cols[10], true);

                recipes.push_back(r);
            } catch (...) {
                continue;
            }
        }

        file.close();
        return recipes;
    }
};