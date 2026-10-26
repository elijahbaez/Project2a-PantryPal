#pragma once
#include <string>
#include <vector>

// Node for our Linked List (handles chaining for overlaps)
struct HashNode {
    std::string key; // The ingredient
    std::vector<int> recipeIDs;
    HashNode* next;

    HashNode(std::string k) : key(k), next(nullptr) {}
};

class HashMap {
private:
    HashNode** table;
    size_t capacity;

    // DJB2 String Hash Function
    size_t hashFunction(const std::string& key) const {
        size_t hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % capacity;
    }

public:
    // Fixed capacity of a prime number to reduce collisions
    HashMap(size_t cap = 100003) : capacity(cap) {
        table = new HashNode*[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            table[i] = nullptr;
        }
    }

    ~HashMap() {
        for (size_t i = 0; i < capacity; ++i) {
            HashNode* entry = table[i];
            while (entry != nullptr) {
                HashNode* prev = entry;
                entry = entry->next;
                delete prev;
            }
        }
        delete[] table;
    }

    void insert(const std::string& ingredient, int recipeID) {
        size_t hashIndex = hashFunction(ingredient);
        HashNode* entry = table[hashIndex];

        // Traverse chain to see if ingredient already exists
        while (entry != nullptr) {
            if (entry->key == ingredient) {
                // Avoid duplicate recipe IDs for the same ingredient
                if (entry->recipeIDs.empty() || entry->recipeIDs.back() != recipeID) {
                    entry->recipeIDs.push_back(recipeID);
                }
                return;
            }
            entry = entry->next;
        }

        // If not found, insert new node at the head of the chain
        HashNode* newNode = new HashNode(ingredient);
        newNode->recipeIDs.push_back(recipeID);
        newNode->next = table[hashIndex];
        table[hashIndex] = newNode;
    }

    std::vector<int> search(const std::string& ingredient) const {
        size_t hashIndex = hashFunction(ingredient);
        HashNode* entry = table[hashIndex];

        while (entry != nullptr) {
            if (entry->key == ingredient) {
                return entry->recipeIDs;
            }
            entry = entry->next;
        }
        return {}; // Return empty vector if not found
    }
};