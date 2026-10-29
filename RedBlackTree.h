#pragma once
#include <string>
#include <vector>

enum Color { RED, BLACK };
struct RBTNode {
    std::string key;
    std::vector<int> recipeIDs;
    Color color;
    RBTNode *left, *right, *parent;
    RBTNode(std::string k) : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    RBTNode* root;
public:
    RedBlackTree() : root(nullptr) {}
    void insert(const std::string& ingredient, int recipeID) { /* TODO */ }
    std::vector<int> search(const std::string& ingredient) const { return {}; }
};