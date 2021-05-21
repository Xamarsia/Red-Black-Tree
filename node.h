#ifndef __NODE_H__
#define __NODE_H__
#include "colors.h"

template <class Key, class Value>
class Node
{
public:
    Node(Key key, Value value) : m_key(key), m_value(value), m_color(Colors::RED) {}
    Node(Key key, Value value, Colors color) : m_key(key), m_value(value), m_color(color) {}

    Key m_key;
    Value m_value;
    Node *m_prev = nullptr;
    Node *m_left = nullptr;
    Node *m_right = nullptr;
    Colors m_color = Colors::RED;
};
#endif