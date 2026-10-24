#pragma once
#include <string>
#include <vector>

struct HashNode {
    std::string key;
    std::vector<int> recipeIDs;
    HashNode* next;
    HashNode(std::string k) : key(k), next(nullptr) {}
};

class HashMap {
private:
    HashNode** table;
    size_t capacity;
    size_t hashFunction(const std::string& key) const {
        size_t hash = 5381;
        for (char c : key) hash = ((hash << 5) + hash) + c; 
        return hash % capacity;
    }
public:
    HashMap(size_t cap = 100003) : capacity(cap) {}
    void insert(const std::string& ingredient, int recipeID) { /* TODO */ }
    std::vector<int> search(const std::string& ingredient) const { return {}; }
};