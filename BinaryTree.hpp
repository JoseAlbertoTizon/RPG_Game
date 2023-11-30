//
// Created by eduardo on 30/11/2023.
//
#pragma once

#ifndef RPG_BINARYTREE_HPP
#define RPG_BINARYTREE_HPP

#include "game.hpp"
#include <queue>

struct BT{
    bool was_bought = false;
    Game::PauseOption boost;

    BT *lchild;
    BT *rchild;
};

void destroy(BT*);

class BinaryTree {
public:
    BinaryTree();
    ~BinaryTree();
    std::vector<bool> all_bought;


    void filling_tree();
    void LevelWalking();
    BT* last_node_of_level(int);
    BT* root = nullptr;
};

#endif //RPG_BINARYTREE_HPP