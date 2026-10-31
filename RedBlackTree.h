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

    void rotateLeft(RBTNode*& root, RBTNode*& pt) {
        RBTNode* pt_right = pt->right;
        pt->right = pt_right->left;
        if (pt->right != nullptr)
            pt->right->parent = pt;
        pt_right->parent = pt->parent;
        if (pt->parent == nullptr)
            root = pt_right;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_right;
        else
            pt->parent->right = pt_right;
        pt_right->left = pt;
        pt->parent = pt_right;
    }

    void rotateRight(RBTNode*& root, RBTNode*& pt) {
        RBTNode* pt_left = pt->left;
        pt->left = pt_left->right;
        if (pt->left != nullptr)
            pt->left->parent = pt;
        pt_left->parent = pt->parent;
        if (pt->parent == nullptr)
            root = pt_left;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_left;
        else
            pt->parent->right = pt_left;
        pt_left->right = pt;
        pt->parent = pt_left;
    }

    void fixViolation(RBTNode*& root, RBTNode*& pt) {
        RBTNode* parent_pt = nullptr;
        RBTNode* grand_parent_pt = nullptr;

        while ((pt != root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
            parent_pt = pt->parent;
            grand_parent_pt = pt->parent->parent;

            if (parent_pt == grand_parent_pt->left) {
                RBTNode* uncle_pt = grand_parent_pt->right;

                if (uncle_pt != nullptr && uncle_pt->color == RED) {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                } else {
                    if (pt == parent_pt->right) {
                        rotateLeft(root, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateRight(root, grand_parent_pt);
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            } else {
                RBTNode* uncle_pt = grand_parent_pt->left;

                if ((uncle_pt != nullptr) && (uncle_pt->color == RED)) {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                } else {
                    if (pt == parent_pt->left) {
                        rotateRight(root, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateLeft(root, grand_parent_pt);
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
        }
        root->color = BLACK;
    }

    void destroyTree(RBTNode* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    RedBlackTree() : root(nullptr) {}

    ~RedBlackTree() {
        destroyTree(root);
    }

    void insert(const std::string& ingredient, int recipeID) {
        RBTNode* pt = new RBTNode(ingredient);
        pt->recipeIDs.push_back(recipeID);

        RBTNode* y = nullptr;
        RBTNode* x = this->root;

        while (x != nullptr) {
            y = x;
            if (pt->key == x->key) {
                if (x->recipeIDs.empty() || x->recipeIDs.back() != recipeID) {
                    x->recipeIDs.push_back(recipeID);
                }
                delete pt;
                return;
            } else if (pt->key < x->key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        pt->parent = y;
        if (y == nullptr) {
            root = pt;
        } else if (pt->key < y->key) {
            y->left = pt;
        } else {
            y->right = pt;
        }

        if (pt->parent == nullptr) {
            pt->color = BLACK;
            return;
        }
        if (pt->parent->parent == nullptr) {
            return;
        }

        fixViolation(root, pt);
    }

    std::vector<int> search(const std::string& ingredient) const {
        RBTNode* curr = root;
        while (curr != nullptr) {
            if (ingredient == curr->key)
                return curr->recipeIDs;
            else if (ingredient < curr->key)
                curr = curr->left;
            else
                curr = curr->right;
        }
        return {};
    }
};