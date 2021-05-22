#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <vector>

enum class Colors
{
    RED,
    BLACK
};

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

template <class Key, class Value>
class Tree
{
private:
    int m_blackLen;
    Node<Key, Value> *m_root = nullptr;

    Node<Key, Value> *parent(Node<Key, Value> *node) const //дедушка
    {
        return node->m_prev;
    }

    Node<Key, Value> *sibling(Node<Key, Value> *node) const //дедушка
    {
        if (node->m_prev == nullptr)
        {
            return nullptr;
        }
        else if (node->m_prev->m_left == node)
        {
            return node->m_prev->m_right;
        }
        return node->m_prev->m_left;
    }

    Node<Key, Value> *grandparent(Node<Key, Value> *node) const //дедушка
    {
        return node->m_prev->m_prev;
    }

    Node<Key, Value> *uncle(Node<Key, Value> *node) const // дядя
    {
        if (node->m_prev->m_prev == nullptr || node->m_prev == nullptr)
        {
            return nullptr;
        }

        else if (node->m_prev->m_prev->m_key < node->m_prev->m_key)
        {
            return node->m_prev->m_prev->m_left;
        }
        return node->m_prev->m_prev->m_right;
    }

    Node<Key, Value> *max(Node<Key, Value> *node1, Node<Key, Value> *node2, Node<Key, Value> *node3) const
    {
        if (node1->m_key > node2->m_key && node1->m_key > node3->m_key)
        {
            return node1;
        }

        else if (node2->m_key > node1->m_key && node2->m_key > node3->m_key)
        {
            return node2;
        }
        return node3;
    }

    Node<Key, Value> *min(Node<Key, Value> *node1, Node<Key, Value> *node2, Node<Key, Value> *node3) const
    {
        if (node1->m_key < node2->m_key && node1->m_key < node3->m_key)
        {
            return node1;
        }

        else if (node2->m_key < node1->m_key && node2->m_key < node3->m_key)
        {
            return node2;
        }
        return node3;
    }

    Node<Key, Value> *average(Node<Key, Value> *node1, Node<Key, Value> *node2, Node<Key, Value> *node3) const
    {

        Node<Key, Value> *maxi = this->max(node1, node2, node3);
        Node<Key, Value> *mini = this->min(node1, node2, node3);
        if (node1->m_key != maxi->m_key && node1->m_key != mini->m_key)
        {
            return node1;
        }
        else if (node2->m_key != maxi->m_key && node2->m_key != mini->m_key)
        {
            return node2;
        }
        return node3;
    }

    void keys(Node<Key, Value> *node, std::vector<Key> &keysArray) const
    {
        keysArray.push_back(node->m_key);
        if (node->m_left == nullptr && node->m_right == nullptr)
        {
            return;
        }
        else if (node->m_left != nullptr && node->m_right != nullptr)
        {
            keys(node->m_left, keysArray);
            keys(node->m_right, keysArray);
        }
        else if (node->m_left == nullptr)
        {
            keys(node->m_right, keysArray);
        }
        else
        {
            keys(node->m_left, keysArray);
        }
    }

    void updateNode(Node<Key, Value> *node, Key &key,
                    Value value, Node<Key, Value> *prev,
                    Node<Key, Value> *left, Node<Key, Value> *right, Colors color)
    {
        node->m_prev = prev;
        node->m_key = key;

        node->m_value = value;
        node->m_left = left;
        node->m_right = right;

        node->m_color = color;
    }

public:
    Tree() : m_blackLen(0) {}
    using KeyType = Key;
    //-------------------------------------------------------------Add new node function -----------------------------------------------------------------------

    void rotation(Node<Key, Value> *node)
    {
        Node<Key, Value> *parent = this->parent(node);
        Node<Key, Value> *grandparent = this->grandparent(node);
        Node<Key, Value> *grangrandparent = this->grandparent(node)->m_prev;

        Node<Key, Value> *average = this->average(parent, node, grandparent);
        Node<Key, Value> *mini = this->min(parent, node, grandparent);
        Node<Key, Value> *maxi = this->max(parent, node, grandparent);

        updateNode(average, average->m_key, average->m_value, grangrandparent, mini, maxi, Colors::BLACK);

        updateNode(maxi, maxi->m_key, maxi->m_value, average, nullptr, nullptr, Colors::RED);
        updateNode(mini, mini->m_key, mini->m_value, average, nullptr, nullptr, Colors::RED);

        if (grangrandparent == nullptr)
        {
            m_root = average;
        }

        else
        {
            if (grangrandparent < average)
            {
                grangrandparent->m_right = average;
            }
            else
            {
                grangrandparent->m_left = average;
            }
        }
    }

    void recoloring(Node<Key, Value> *node)
    {
        Node<Key, Value> *uncle = this->uncle(node);
        Node<Key, Value> *parent = this->parent(node);
        Node<Key, Value> *grandparent = this->grandparent(node);

        if (uncle->m_color == Colors::RED && grandparent->m_color == Colors::BLACK)
        {
            uncle->m_color = Colors::BLACK;
            parent->m_color = Colors::BLACK;
            grandparent->m_color = Colors::RED;
            if (grandparent->m_prev != nullptr && grandparent->m_prev->m_prev != nullptr)
            {
                recoloring(grandparent);
            }
            if (m_root->m_color == Colors::RED)
            {
                m_root->m_color = Colors::BLACK;
                m_blackLen = m_blackLen + 1;
            }
        }
    }

    void ll_rotation(Node<Key, Value> *node)
    {
        Node<Key, Value> *uncle = this->uncle(node);
        Node<Key, Value> *parent = this->parent(node);
        Node<Key, Value> *grandparent = this->grandparent(node);
        Node<Key, Value> *grangrandparent = this->grandparent(node)->m_prev;

        if (grandparent == m_root)
        {
            m_root = parent;
        }

        grandparent->m_left = parent->m_right;
        grandparent->m_color = Colors::RED;

        if (grangrandparent != nullptr)
        {
            updateNode(parent, parent->m_key, parent->m_value, grangrandparent, node, grandparent, Colors::BLACK);
        }
        else
        {
            updateNode(parent, parent->m_key, parent->m_value, nullptr, node, grandparent, Colors::BLACK);
        }

        grandparent->m_prev = parent;
    }

    void lr_rotation(Node<Key, Value> *node)
    {
        Node<Key, Value> *uncle = this->uncle(node);
        Node<Key, Value> *parent = this->parent(node);
        Node<Key, Value> *grandparent = this->grandparent(node);
        Node<Key, Value> *T3 = parent->m_left;

        grandparent->m_left = node;

        parent->m_left = node->m_left;
        parent->m_right = node->m_right;
        parent->m_prev = node;

        node->m_left = parent;
        node->m_right = T3;
        node->m_prev = grandparent;

        ll_rotation(node);
    }

    void rr_rotation(Node<Key, Value> *node)
    {
        Node<Key, Value> *uncle = this->uncle(node);
        Node<Key, Value> *parent = this->parent(node);
        Node<Key, Value> *grandparent = this->grandparent(node);

        if (grandparent == m_root)
        {
            m_root = parent;
        }
        parent->m_color = Colors::BLACK;
        grandparent->m_color = Colors::RED;
        if (grandparent->m_prev != nullptr)
        {
            parent->m_prev = nullptr;
        }
        else
        {
            parent->m_prev = grandparent->m_prev;
        }
        grandparent->m_right = parent->m_left;

        grandparent->m_prev = parent;
        parent->m_left = grandparent;
        grandparent->m_right->m_prev = grandparent;
    }

    void rl_rotation(Node<Key, Value> *node)
    {
        Node<Key, Value> *uncle = this->uncle(node);
        Node<Key, Value> *parent = this->parent(node);
        Node<Key, Value> *grandparent = this->grandparent(node);
        Node<Key, Value> *T3 = parent->m_right;

        grandparent->m_right = node;

        parent->m_left = node->m_right;
        parent->m_prev = node;

        node->m_right = parent;
        node->m_prev = grandparent;

        rr_rotation(node);
    }

    void balancing(Node<Key, Value> *node)
    {
        Node<Key, Value> *parent = this->parent(node);
        Node<Key, Value> *grandparent = this->grandparent(node);

        //Выполните следующие действия, если цвет родительского элемента x не ЧЕРНЫЙ, а x не является корнем.
        if (grandparent != nullptr && parent != nullptr && parent->m_color == Colors::RED)
        {
            Node<Key, Value> *uncle = this->uncle(node);
            if (uncle == nullptr && grandparent->m_color == Colors::BLACK)
            {
                rotation(node);
            }
            else if (uncle->m_color == Colors::RED && grandparent->m_color == Colors::BLACK)
            {
                recoloring(node);
            }
            else if (uncle->m_color == Colors::BLACK)
            {
                if (grandparent->m_left == parent && parent->m_left == node)
                {
                    ll_rotation(node);
                }
                if (grandparent->m_left == parent && parent->m_right == node)
                {
                    lr_rotation(node);
                }
                if (grandparent->m_right == parent && parent->m_right == node)
                {
                    rr_rotation(node);
                }
                if (grandparent->m_right == parent && parent->m_left == node)
                {
                    rl_rotation(node);
                }
            }
        }
    }

    void rotationRecolor(Node<Key, Value> *node)
    {
        if (node->m_prev != nullptr && node->m_prev->m_prev != nullptr)
        {
            Node<Key, Value> *parent = this->parent(node);
            Node<Key, Value> *grandparent = this->grandparent(node);
            if (grandparent != nullptr && parent != nullptr && parent->m_color == Colors::RED)
            {
                Node<Key, Value> *uncle = this->uncle(node);
                if (uncle != nullptr && uncle->m_color == Colors::BLACK)
                {
                    if (node->m_color == Colors::RED && grandparent->m_color == Colors::BLACK)
                    {
                        balancing(node);
                    }
                }
            }
        }

        while (node->m_prev != nullptr && node->m_prev->m_prev != nullptr)
        {
            node = node->m_prev;
            rotationRecolor(node);
        }
    }

    void add(const Key &key, const Value &value)
    {
        if (m_root == nullptr) // если корень пуст
        {
            Node<Key, Value> *node = new Node<Key, Value>(key, value, Colors::BLACK);
            m_root = node;
            m_blackLen = 1;
        }

        else
        {
            Node<Key, Value> *root = m_root;
            Node<Key, Value> *prew;
            Node<Key, Value> *node = new Node<Key, Value>(key, value);
            while (root != nullptr)
            {
                if (root->m_left == nullptr || root->m_right == nullptr)
                {
                    prew = root;
                }
                if (root->m_key < key)
                {
                    root = root->m_right;
                }
                else if (root->m_key > key)
                {
                    root = root->m_left;
                }
            }
            // прошлись по дереву от самого корня и нашли будущее положение вставленой ноды
            root = node;
            node->m_prev = prew;
            if (prew->m_key > key)
            {
                prew->m_left = node;
            }
            else
            {
                prew->m_right = node;
            }
            //функция завершена и работает;
            balancing(node);
            rotationRecolor(node);
        }
    }

    Value &operator[](const Key &key)
    {
        //не забудь проработать случай с добавлением нового елемента
        Node<Key, Value> *root = m_root;
        while (root != nullptr)
        {
            if (root->m_key == key)
            {
                return root->m_value;
            }
            else
            {
                if (root->m_key > key)
                {
                    root = root->m_left;
                }
                else
                {
                    root = root->m_right;
                }
            }
        }
        return root->m_value;
    }

    const Value &operator[](const Key &key) const
    {
        Node<Key, Value> *root = m_root;
        while (root != nullptr)
        {
            if (root->m_key == key)
            {
                return root->m_value;
            }
            else
            {
                if (root->m_key > key)
                {
                    root = root->m_left;
                }
                else
                {
                    root = root->m_right;
                }
            }
        }
        return root->m_value;
    }

    Node<Key, Value> *getNode(const Key &key) const
    {
        Node<Key, Value> *root = m_root;
        while (root != nullptr)
        {
            if (root->m_key == key)
            {
                return root;
            }
            else
            {
                if (root->m_key > key)
                {
                    root = root->m_left;
                }
                else
                {
                    root = root->m_right;
                }
            }
        }
        return root;
    }

    //------------------------------------------------------------- Delete node function ---------------------------------------------------------------------

    bool hasRedChild(Node<Key, Value> *node) const
    {
        return (node->m_left != nullptr && node->m_left->m_color == Colors::RED) || (node->m_right != nullptr && node->m_right->m_color == Colors::RED);
    }

    bool twoBlackChild(Node<Key, Value> *node) const
    {
        Node<Key, Value> *sibling = this->sibling(node);
        return (sibling->m_left == nullptr || sibling->m_left->m_color == Colors::BLACK) && (sibling->m_right == nullptr || sibling->m_right->m_color == Colors::BLACK);
    }

    Node<Key, Value> *maxLeftNode(Node<Key, Value> *node) const   // преемник
    {
        if (node == m_root && node->m_left != nullptr)
        {
            node = node->m_left;
        }
        while (node->m_right != nullptr)
        {
            if (node->m_left == nullptr)
            {
                return node;
            }
            node = node->m_right;
        }
        return node;
    }

    void remov(const Key &key)
    {
        Node<Key, Value> *node = getNode(key);
        Node<Key, Value> *parent = this->parent(node);

        if (node == nullptr)
        {
            return;
        }
        // узел с двумя детьми
        else if (node->m_left != nullptr && node->m_right != nullptr)
        {
            //К2 — красный узел с двумя детьми или Ч2 — чёрный узел с двумя детьми
            if (node->m_color == Colors::RED || node->m_color == Colors::BLACK)
            {
                Node<Key, Value> *successor = maxLeftNode(node);
                node->m_key = successor->m_key;
                node->m_value = successor->m_value;
                if (successor->m_right == nullptr)
                {
                    successor->m_prev->m_right = nullptr;
                }
                else
                {
                    successor->m_right->m_color = successor->m_prev->m_right->m_color;
                    successor->m_prev->m_right = successor->m_right;
                    successor->m_right->m_prev = successor->m_prev;
                }
                delete successor;
            }
        }

        // узел с одним ребёнком
        else if (node->m_left != nullptr || node->m_right != nullptr)
        {
            // Ч1 — чёрный узел с одним ребёнком
            if (node->m_color == Colors::BLACK)
            {
                Node<Key, Value> *successor;
                if (node->m_left != nullptr)
                {
                    successor = node->m_left;
                    node->m_left = nullptr;
                }
                else
                {
                    successor = node->m_right;
                    node->m_right = nullptr;
                }
                node->m_key = successor->m_key;
                node->m_value = successor->m_value;
                delete successor;
            }
            //К1 — красный узел с одним ребёнком
            // невозможный вариант
        }
        // узел без детей
        else if (node->m_left == nullptr || node->m_right == nullptr)
        {
            // К0 — красный узел без детей
            if (node->m_color == Colors::RED)
            {
                if (parent->m_right == node)
                {
                    parent->m_right = nullptr;
                    delete node;
                }
                else
                {
                    parent->m_left = nullptr;
                    delete node;
                }
            }
            // Ч0 — чёрный узел без детей
            if (node->m_color == Colors::BLACK)
            {
                deleteBalancing(node);
                if (parent->m_right == node)
                {
                    parent->m_right = nullptr;
                }
                else
                {
                    parent->m_left = nullptr;
                }
                delete node;
            }
        }
    }

    void deleteRecoloring(Node<Key, Value> *node)
    {
        Node<Key, Value> *sibling = this->sibling(node);
        Node<Key, Value> *parent = this->parent(node);
        Node<Key, Value> *grandparent = this->grandparent(node);

        parent->m_color = Colors::BLACK;
        node->m_color = Colors::RED;

        if (sibling != nullptr)
        {
            sibling->m_color = Colors::RED;
        }

        if (grandparent != nullptr && grandparent->m_prev != nullptr)
        {
            deleteRecoloring(grandparent);
        }

        if (m_root->m_color == Colors::RED)
        {
            m_root->m_color = Colors::BLACK;
        }
    }

    void deleteBalancing(Node<Key, Value> *node)
    {
        Node<Key, Value> *parent = this->parent(node);
        Node<Key, Value> *sibling = this->sibling(node);
        Node<Key, Value> *grandparent = this->grandparent(node);

        // родитель красный
        if (parent->m_color == Colors::RED && sibling != nullptr)
        {
            // КЧ1 — родитель красный, левый ребёнок чёрный с чёрными внуками
            if (sibling->m_color == Colors::BLACK && !hasRedChild(sibling))
            {
                deleteRecoloring(node);
            }

            //КЧ2 — родитель красный, левый ребёнок чёрный с левым красным внуком.
            if (sibling->m_color == Colors::BLACK && hasRedChild(sibling))
            {
                sibling->m_prev = grandparent;
                sibling->m_color = Colors::RED;

                parent->m_prev = sibling;
                parent->m_left = sibling->m_right;

                parent->m_right = node;
                parent->m_color = Colors::BLACK;

                sibling->m_right = parent;
                sibling->m_left->m_color = Colors::BLACK;
            }
        }
        // родитель чёрный
        if (parent->m_color == Colors::BLACK && sibling != nullptr)
        {
            if (sibling->m_color == Colors::RED && sibling->m_right != nullptr)
            {
                // ЧК3 — родитель чёрный, левый сын красный, у правого внука чёрные правнуки
                if (twoBlackChild(sibling->m_right))
                {
                    if (grandparent->m_right == parent)
                    {
                        grandparent->m_right = sibling;
                    }
                    else
                    {
                        grandparent->m_left = sibling;
                    }
                    parent->m_left = sibling->m_right;
                    sibling->m_color = Colors::BLACK;
                    sibling->m_right->m_color = Colors::RED;
                    sibling->m_right->m_prev = parent;
                    parent->m_prev = sibling;

                    sibling->m_right = parent;
                    sibling->m_prev = grandparent;
                }

                //ЧК4 — родитель чёрный, левый сын красный, у правого внука левый правнук красный
                if ((sibling->m_right)->m_left->m_color == Colors::RED)
                {
                    Node<Key, Value> *child = sibling->m_right;
                    if (grandparent->m_right == parent)
                    {
                        grandparent->m_right = child;
                    }
                    else
                    {
                        grandparent->m_left = child;
                    }

                    parent->m_left = child->m_right;
                    child->m_right->m_prev = parent;

                    parent->m_prev = child;
                    child->m_right = parent;

                    sibling->m_right = child->m_left;
                    child->m_left->m_prev = sibling;

                    sibling->m_prev = child;
                    child->m_left = sibling;

                    sibling->m_right->m_color = Colors::BLACK;
                }
            }

            if (sibling->m_color == Colors::BLACK && sibling->m_right != nullptr)
            {
                //ЧЧ5 — родитель чёрный, левый сын чёрный с правым красным внуком.
                if ((sibling->m_right)->m_color == Colors::RED)
                {
                    Node<Key, Value> *child = sibling->m_right;

                    if (grandparent->m_right == parent)
                    {
                        grandparent->m_right = child;
                    }
                    else
                    {
                        grandparent->m_left = child;
                    }

                    sibling->m_right = child->m_left;
                    child->m_prev = sibling;

                    child->m_left = sibling;
                    sibling->m_prev = child;

                    parent->m_left = child->m_right;
                    child->m_right->m_prev = parent;

                    parent->m_prev = child;
                    child->m_right = parent;
                    child->m_color = Colors::BLACK;
                }
                //ЧЧ6 — родитель чёрный, левый сын чёрный, его внуки тоже чёрные
                if (twoBlackChild(sibling->m_right))
                {
                    deleteRecoloring(node);
                }
            }
        }
    }

    bool contains(const Key &key) const // проверка есть ли ключ;
    {
        Node<Key, Value> *root = m_root;
        while (root != nullptr)
        {
            if (root->m_key == key)
            {
                return true;
            }
            else
            {
                if (root->m_key > key)
                {
                    root = root->m_left;
                }
                else
                {
                    root = root->m_right;
                }
            }
        }
        return false;
    }

    std::vector<Key> keys() const  // метод что возвращает вектор со всеми ключамиж;
    {
        std::vector<Key> m_keysArray;
        keys(m_root, m_keysArray);
        return m_keysArray;
    }
};

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
    for (auto &nod : array)
    {
    }
}

int main()
{
    std::cout << ".............................................." << std::endl;
    Tree<int, std::string> temp;
    temp.add(8, "11111");
    temp.add(5, "22222");
    temp.add(18, "33333");
    temp.add(15, "44444");
    temp.add(17, "55555");
    temp.add(25, "66666");
    temp.add(40, "77777");
    temp.add(80, "88888");
    temp.add(4, "99999");
    temp.add(6, "000000");
    std::cout << temp[8] << std::endl;
    auto t = temp.keys();
    const std::vector<int> &p = t;
    print<int>(p);

    keyValue<int, std::string>(temp);

    std::cout << "\n"
              << temp.contains(999) << " " << temp.contains(6) << std::endl;
}