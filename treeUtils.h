#ifndef __TREE_UTILS_H__
#define __TREE_UTILS_H__
#include "tree.h"

template <typename Key>
void print(const std::vector<Key> &array)
{
    unsigned int len = array.size();
    for (int i = 0; i < len; i++)
    {
        std::cout << array[i] << " ";
    }
}

template <typename Key, typename Value>
void keyValue(const Tree<Key, Value> &temp)
{
    auto array = temp.keys();
    unsigned int len = array.size();
    for (int i = 0; i < len; i++)
    {
        std::cout << array[i] << " " << temp[array[i]] << " \n";
    }
}

#endif