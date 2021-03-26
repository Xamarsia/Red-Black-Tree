#include <iostream>
#include <typeinfo>
#include <algorithm>

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
    int m_blackLen; // длина массива;
    Node<Key, Value> *m_root = nullptr;

    Node<Key, Value> *Parent(Node<Key, Value> *node) //дедушка
    {
        return node->m_prev;
    }

    Node<Key, Value> *Sibling(Node<Key, Value> *node) //дедушка
    {
        Node<Key, Value> *parent = Parent(node);
        if (parent == nullptr)
        {
            return nullptr;
        }
        else if (parent->m_left == node)
        {
            return parent->m_right;
        }
        return parent->m_left;
    }

    Node<Key, Value> *Grandparent(Node<Key, Value> *node) //дедушка
    {
        return node->m_prev->m_prev;
    }

    Node<Key, Value> *Uncle(Node<Key, Value> *node) // дядя
    {

        Node<Key, Value> *parent = Parent(node);
        Node<Key, Value> *grandparent = Grandparent(node);

        if (grandparent == nullptr || parent == nullptr)
        {
            return nullptr;
        }

        else if (grandparent->m_key < parent->m_key)
        {
            return grandparent->m_left;
        }
        return grandparent->m_right;
    }

    Node<Key, Value> *Max(Node<Key, Value> *node1, Node<Key, Value> *node2, Node<Key, Value> *node3) // сравнение
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

    Node<Key, Value> *Min(Node<Key, Value> *node1, Node<Key, Value> *node2, Node<Key, Value> *node3)
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

    Node<Key, Value> *Average(Node<Key, Value> *node1, Node<Key, Value> *node2, Node<Key, Value> *node3)
    {

        Node<Key, Value> *maxi = Max(node1, node2, node3);
        Node<Key, Value> *mini = Min(node1, node2, node3);
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

    void updateNode(Node<Key, Value> *node, Key key,
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

    //-------------------------------------------------------------Add new node function -----------------------------------------------------------------------

    void rotation(Node<Key, Value> *node)
    {
        Node<Key, Value> *parent = Parent(node);
        Node<Key, Value> *grandparent = Grandparent(node);
        Node<Key, Value> *grangrandparent = Grandparent(node)->m_prev;

        Node<Key, Value> *average = Average(parent, node, grandparent);
        Node<Key, Value> *mini = Min(parent, node, grandparent);
        Node<Key, Value> *maxi = Max(parent, node, grandparent);

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
        // а) Если дядя x КРАСНЫЙ (дедушка и бабушка должны были быть черными из свойства 4)
        // (i) Измените цвет родителя и дяди на ЧЕРНЫЙ.
        // (ii) Цвет бабушки и дедушки - КРАСНЫЙ.
        // (iii) Замените прародителя x = x, повторите шаги 2 и 3 для нового x.

        Node<Key, Value> *uncle = Uncle(node);
        Node<Key, Value> *parent = Parent(node);
        Node<Key, Value> *grandparent = Grandparent(node);

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
        Node<Key, Value> *uncle = Uncle(node);
        Node<Key, Value> *parent = Parent(node);
        Node<Key, Value> *grandparent = Grandparent(node);
        Node<Key, Value> *grangrandparent = Grandparent(node)->m_prev;

        if (grandparent == m_root)
        {
            m_root = parent;
        }

        grandparent->m_left = parent->m_right; // G
        grandparent->m_color = Colors::RED;

        if (grangrandparent != nullptr) // P
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
        Node<Key, Value> *uncle = Uncle(node);
        Node<Key, Value> *parent = Parent(node);
        Node<Key, Value> *grandparent = Grandparent(node);
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
        Node<Key, Value> *uncle = Uncle(node);
        Node<Key, Value> *parent = Parent(node);
        Node<Key, Value> *grandparent = Grandparent(node);
        // x and y not neet apdate;

        if (grandparent == m_root)
        {
            m_root = parent;
        }
        parent->m_color = Colors::BLACK;
        grandparent->m_color = Colors::RED;
        if (grandparent->m_prev != nullptr) // P
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
    }

    void rl_rotation(Node<Key, Value> *node)
    {
        Node<Key, Value> *uncle = Uncle(node);
        Node<Key, Value> *parent = Parent(node);
        Node<Key, Value> *grandparent = Grandparent(node);
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
        Node<Key, Value> *parent = Parent(node);
        Node<Key, Value> *grandparent = Grandparent(node);

        //Выполните следующие действия, если цвет родительского элемента x не ЧЕРНЫЙ, а x не является корнем.
        if (grandparent != nullptr && parent != nullptr && parent->m_color == Colors::RED)
        {
            Node<Key, Value> *uncle = Uncle(node);
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
            Node<Key, Value> *parent = Parent(node);
            Node<Key, Value> *grandparent = Grandparent(node);
            if (grandparent != nullptr && parent != nullptr && parent->m_color == Colors::RED)
            {
                Node<Key, Value> *uncle = Uncle(node);
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

    void add(Key key, Value value)
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

    Value &operator[](Key key)
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

    Node<Key, Value> *returnNode(Key key)
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

    // check if node is left child of parent
    bool isLeft(Node<Key, Value> *node)
    {
        return node == node->m_prev->left;
    }

    bool hasRedChild(Node<Key, Value> *node)
    {
        return (node->m_left != nullptr && node->m_left == Colors::RED) || (node->m_right != nullptr && node->m_right == Colors::RED);
    }

    void standartDelete(Key key)
    {
        if (m_root == nullptr)
        {
            return;
        }

        Node<Key, Value> *node = returnNode(key);
        if (node != nullptr)
        {
            deleteNode(node);
        }
    }

    void deleteNode(Node<Key, Value> *node)
    {
    }

    Node<Key, Value> *successor(Node<Key, Value> *node)
    {
        Node<Key, Value> *temp = node;
        while (temp->m_left != nullptr)
        {
            temp = temp->m_left;
        }
        return temp;
    }

    // найти узел, который заменяет удаленный узел в
    Node<Key, Value> *replace(Node<Key, Value> *node)
    {
        // когда у узла есть 2 ребенка
        if (node->m_right != nullptr && node->m_left != nullptr)
        {
            // найти узел, у которого нет левого потомка в поддереве данного узла
            return successor(node->m_right);
        }
        // когда лист
        if (node->m_right == nullptr && node->m_left == nullptr)
        {
            return nullptr;
        }
        // когда одинокий ребенок
        if (node->m_left != nullptr)
        {
            return node->m_left;
        }
        else
        {
            return node->m_right;
        }
    }

    // нам нужно обрабатывать только те случаи, когда узел является листовым или имеет одного дочернего элемента.
    void Delete(Key key)
    {
        Node<Key, Value> *node = returnNode(key);
        Node<Key, Value> *parent = Parent(node);

        Node<Key, Value> *replaceNode = replace(node); // u
        
        if (node->m_right == nullptr && node->m_left == nullptr)
        {

            if (parent->m_left == node)
            {
                parent->m_left = nullptr;
            }
            else
            {
                parent->m_right = nullptr;
            }
            delete node;
        }
        // если parent или node красный, мы помечаем замененный дочерний элемент
        // черным (без изменения высоты черного
        if (node->m_color == Colors::RED || parent->m_color == Colors::RED)
        {
            parent->m_color == Colors::BLACK;
            standardDelete(node);
        }
        // Если и u, и v черные . v = node , u node->next
        if (node->m_color == Colors::BLACK)
        {
            if (node->m_left == nullptr || node->m_right == nullptr)
            {
                if (node->m_right == nullptr && node->m_left->m_color == Colors::BLACK)
                {
                    doubleBlack(node);
                }
                else if (node->m_left == nullptr && node->m_right->m_color == Colors::BLACK)
                {
                    doubleBlack(node);
                }
            }
            if (node->m_left == nullptr && node->m_right == nullptr)
            {
                doubleBlack(node);
            }
        }
    }
};

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
    std::cout << temp[8] << std::endl;
}