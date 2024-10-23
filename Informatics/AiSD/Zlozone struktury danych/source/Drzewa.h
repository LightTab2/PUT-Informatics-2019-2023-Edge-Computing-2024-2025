#pragma once
#include <math.h>
#include <chrono>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
using namespace std;

struct BST
{
    BST(unsigned long long val = 0, BST *parent = nullptr)
    {
        this->val = val;
        this->parent = parent;
    }
    unsigned long long val = 0;
    BST *left = nullptr, *right = nullptr, *parent = nullptr;
};

struct AVL
{
    AVL(unsigned long long val = 0, AVL* parent = nullptr, int rownowaga = 0)
    {
        this->val = val;
        this->parent = parent;
        this->rownowaga = rownowaga;
    }
    unsigned long long val = 0;
    int rownowaga = 0;
    AVL *left = nullptr, *right = nullptr, *parent = nullptr;
};

BST *bst = nullptr;
AVL *avl = nullptr;
bool avl_zrownowazone = true;
AVL *avl_binarnie = nullptr;