//
// Created by eduardo on 30/11/2023.
//

#include <iostream>
#include "BinaryTree.hpp"

BinaryTree::BinaryTree(){
    root = nullptr;
    std::cout << "here\n";
    filling_tree();
}

BinaryTree::~BinaryTree() {
    destroy(root);
}

void destroy(BT* root) {
    if(root == nullptr)
        return;
    destroy(root->lchild);
    destroy(root->rchild);
    delete root;
}

void BinaryTree::filling_tree() {
    std::cout << "1\n";
    root = new BT;
    root->lchild = new BT;
    root->rchild = new BT;
    root->lchild->lchild = new BT;
    root->lchild->rchild = new BT;
    root->rchild->lchild = new BT;
    root->rchild->rchild = new BT;
    std::cout << "2\n";

    for(int i = 0; i < 3; ++ i) {
        all_bought.push_back(false);
    }
    std::cout << "3\n";

    root->was_bought = false;
    root->lchild->was_bought = false;
    root->rchild->was_bought = false;
    root->lchild->lchild->was_bought = false;
    root->lchild->rchild->was_bought = false;
    root->rchild->lchild->was_bought = false;
    root->rchild->rchild->was_bought = false;
    std::cout << "4\n";


    root->boost = Game::AddSpeed;
    root->lchild->boost = Game::AddDamage;
    root->rchild->boost = Game::AddDamage;
    root->lchild->lchild->boost = Game::AddSpeed;
    root->lchild->rchild->boost = Game::AddSpeed;
    root->rchild->lchild->boost = Game::AddDamage;
    root->rchild->rchild->boost = Game::AddDamage;
    std::cout << "got here\n";
}

void BinaryTree::LevelWalking() {
    if(root == nullptr)
        return;

    std::queue<BT*> queue;
    queue.push(this->root);

    int level = 0;
    while(not queue.empty()){
        BT* actual = queue.front();
        queue.pop();
        if(not actual->was_bought) {
            all_bought[level] = false;
            break;
        }

        if(actual == last_node_of_level(level)) {
            ++level;
            all_bought[level] = true;
        }

        if(actual->lchild != nullptr)
            queue.push(actual->lchild);
        if(actual->rchild != nullptr)
            queue.push(actual->rchild);
   }
}

BT* BinaryTree::last_node_of_level(int level) {
    BT* last = root;
    for(int i = 0; i < level; ++ i) {
        last = root->rchild;
    }
    return last;
}
