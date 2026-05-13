#include <vector>

/* TODO: tell about B+-tree, B*-tree. */

template <typename T>
class BTree {
public:
    struct Node {
        Node (bool leaf) : leaf(leaf) {}
        ~Node()
        {
            for (Node *child: children)
                delete child;
        }

        bool leaf;
        /* N ключей - > N+1 ребенок. */
        std::vector<T> keys;
        std::vector<Node *> children;
    };

    BTree(size_t min_degree): t(min_degree), root(nullptr) {}

    /* Используем оптимизацию "расщепление во время прохода". */
    void
    Insert(const T &key)
    {
        if (root == nullptr)
            root = Node(/*leaf=*/true);

        if (isNodeFull(root))
        {
            /* Корень переполнен - сразу содзаем новый. */
            Node *newRoot = new Node(/*leaf=*/false);
            newRoot->children.push_back(root);
            root = newRoot;

            /* Расщепляем старый корень на два поддерева. */
            splitChild(root, 0);
        }

        insertNonFull(root, key);
    }

    void
    Has(const T &key)
    {/* NYI */}

private:

    bool
    isNodeFull(Node *node)
    {
        return node->keys.size() == 2 * t - 1;
    }

    /* Разбиваем потомка с индексом `index` узла `root` на два поддерева. */
    void
    splitChild(Node *node, size_t index);

    void
    InsertNonFull(Node *node, const T& key)
    {
        int pos = node->keys.size() - 1;

        if (node->left) {
            /* Случай с листом. */
            /* Резервируем место под новый ключ. */
            node->keys.resize(node->keys.size() + 1);

            /*
             * Идем справа налево, ищем позицию вставки
             * Так как ключи должны быть упорядочены!!!
             *
             * Для оптимизации - бин.поиск + memmove.
             * Упорядочивание от меньшего к большему.
             */
            while (pos >= 0 && key < node->keys[pos]) {
                /* Двигаем элементы больше ключа вправо. */
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            node->keys[pos + 1] = key;
        } else {
            /* Случай с внутренним узлом. */
            while (pos >= 0 && key < node->keys[pos])
                pos--;

            if (isNodeFull(node->children[pos + 1])) {
                /* Расщепляем, чтобы работать с неполными блоками. */
                splitChild(node, pos + 1);

                /* После расщепления к нам поднялась медиана - нам нужно тоже с ней сравниться. */
                if (key > node->keys[pos + 1])
                    pos++;
            }
            InsertNonFull(node->children[pos + 1], key);
        }
    }

    Node *root;
    /* Minimal amount of children. */
    size_t t;
};
