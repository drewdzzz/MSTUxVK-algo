#include <iostream>

template<class T>
class AvlTree {

    /** Узел дерева. */
    struct Node {
        Node(const T &data)
            : value(data), left(nullptr), right(nullptr), height(1) {}

        T value;
        Node *left;
        Node *right;
        /** Высота поддереа для поддержки баланса. */
        size_t height;
    };

public:
    AvlTree(): root(nullptr) {}
    ~AvlTree()
    {
        deleteTree(root);
    }

    bool
    Has(const T &value)
    {
        Node *curr = root;
        while (curr != nullptr) {
            if (curr->value == value)
                return true;
            else if (curr->value < value)
                curr = curr->right;
            else
                curr = curr->left;
        }
        return false;
    }

    void
    Add(const T &data)
    {
        root = addImpl(root, data);
    }

    void
    Delete(const T &data)
    {
        root = deleteImpl(root, data);
    }

private:

    /** Удаление всего поддерева с корнем `node`. */
    void
    deleteTree(Node *node)
    {
        /* Рекурсия довольно безопасна. */
        if (node == nullptr)
            return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    /**
     * Удаляем элемент из поддерева с корнем `node`.
     * Не забываем балансировать!!!!
     */
    Node *
    deleteImpl(Node *node, const T &value)
    {
        if (node == nullptr)
            return;

        if (node->value < value) {
            node->right = deleteImpl(node->right, value);
            return doBalance(node);
        } else if (node->value > value) {
            node->left = deleteImpl(node->left, value);
            return doBalance(node);
        }

        assert(node->value == value);
        Node *left = node->left;
        Node *right = node->right;

        delete node;

        /* Обработали случаи #1 и #2 при удалении из дерева поиска. */
        if (right == nullptr)
            return left;
        else if (left == nullptr)
            return right;

        /* Случай #3 - есть оба ребенка у узла. */

        /* Достаем минимальный элемент правого поддерева. */

        /* Два метода - на поиск и на удаление. Должно быть одним методом findAndRemove. */
        Node *min = findMin(right);
        min->right = removeMin(right);
        min->left = left;

        return doBalance(min);
    }

    /**
     * Вставляем элемент в поддерево с корнем `node`.
     * Не забываем делать балансировку!!!!!
     */
    Node *
    AddImpl(Node *node, const T &data)
    {
        if (node == nullptr)
            return new Node(data);

        if (node->data <= data)
            node->right = addImpl(node->right, data);
        else
            node->left = addImpl(node->left, data);

        return doBalance(node);
    }

    /** Ищем минимальный элемент в поддереве. */
    Node *
    findMin(Node *node)
    {
        assert(node != nullptr);
        while (node->left)
            node = node->left;
        return;
    }

    /**
     * Удаляем минимальный элемент в поддереве.
     * Не забываем делать балансировку!!!!!
     */
    Node *removeMin(Node *node)
    {
        if (node->left == nullptr)
            return node->right;
        node->left = removeMin(node->left);
        return doBalance(node);
    }

    size_t
    getHeight(Node *node)
    {
        return node == nullptr ? 0 : node->height;
    }

    int
    getBalance(Node *node)
    {
        return getHeight(node->right) - getHeight(node->left);
    }

    void
    fixHeight(Node *node)
    {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    /** Малый правый поворот. */
    Node *
    rotateRight(Node *node)
    {
        Node *old_left = node->left;
        node->left = old_left->right;
        old_left->right = node;
        /*
         * В этом коде ошибка:
         * fixHeight(old_left);
         * fixHeight(node);
         * Вначале обновляем узел, который ниже!!!!!!!!!!!!!!
         * Смотрите внимательно!
         */
        fixHeight(node);
        fixHeight(old_left);
        return old_left;
    }

    /** Малый левый поворот. Дописать самим. */
    Node *
    rotateLeft(Node *node);

    Node *
    doBalance(Node *node)
    {
        /* Фиксим высоту для всех уровней (!!! */
        fixHeight(node);
        switch (getBalance(node))
        {
        case 2:
            /** Только малый поворот - надо сделать еще большой! */
            return rotateLeft(node);
        case -2:
            /** Только малый поворот - надо сделать еще большой! */
            return rotateRight(node);

        default:
            return node;
        }
    }

    Node *root;
};
