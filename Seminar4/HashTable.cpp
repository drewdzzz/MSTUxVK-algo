/**
 * Хэш-таблица методом цепочек.
 */

#include <iostream>
#include <vector>

/**
 * |-|
 * | | <- List Node
 * |-|
 * | |
 * |-|
 * ^
 * |
 * Array of List Nodes.
 */

template <typename T>
struct HashTableNode {
    HashTableNode(): next(nullptr) {}

    /* Максимально универсальный способ принять аргумент - const &. */
    HashTableNode(const T &data, HashTableNode<T> *next):
        data(data), next(next) {}

    T data;
    HashTableNode<T> *next;
};

static const size_t HASH_TABLE_INITIAL_SIZE = 16;

/** HashTable-based set (no values). */
template <typename T, typename Hasher>
class HashTable {
public:
    HashTable(): size(0), table(HASH_TABLE_INITIAL_SIZE) {}
    /* Правило трех - не забываем (я забыл((( ). */

    ~HashTable()
    {
        for (size_t i = 0; i < table.size(); i++) {
            HashTableNode<T> *node = table[i];

            while (node != nullptr) {
                HashTableNode<T> *next = node->next;
                delete node;
                node = next;
            }
        }
    }

    /* Returns true on success, false if there is a duplicate. */
    bool
    Add(const T &key)
    {
        /* Rehash, if needed. */
        if (NeedGrow())
            Grow();

        /* Find slot. */
        size_t index = hasher(key) % table.size();
        HashTableNode<T> *node = table[index];

        /* Search for duplicate. */
        while (node != nullptr) {
            if (node->data == key) {
                return false;
            }
            node = node->next;
        }

        /* No duplicate - insert the new key. */
        table[index] = new HashTableNode<T>(key, table[index]);
        size++;
        return true;
    }

    bool
    Has(const T &key)
    {
        /* Find slot. */
        size_t index = hasher(key) % table.size();
        HashTableNode<T> *node = table[index];

        /* Search for the element. */
        while (node != nullptr) {
            if (node->data == key) {
                return true;
            }
            node = node->next;
        }
        return false;
    }

    /* Returns true on success, returns false if there is no such key. */
    bool
    Delete(const T &key)
    {
        /* Find slot. */
        size_t index = hasher(key) % table.size();
        HashTableNode<T> *node = table[index];
        HashTableNode<T> *prev = nullptr;

        /* Search for the key. */
        while (node != nullptr) {
            if (node->data == key) {
                break;
            }
            prev = node;
            node = node->next;
        }

        /* Not found. */
        if (node == nullptr)
            return false;

        /* Turn [prev]->[node]->[next] into [prev]->[next]. */
        if (prev == nullptr)
            table[index] = node->next;
        else
            prev->next = node->next;

        delete node;
        size--;
        return true;
    }

private:

    bool
    NeedGrow()
    {
        const size_t MAX_LOAD_FACTOR = 2;
        return size > table.size() * MAX_LOAD_FACTOR;
    }

    void
    Grow()
    {
        /* Create new storage. */
        std::vector<HashTableNode<T> *> newTable(table.size() * 2, nullptr);

        /* Iterator over all slots. */
        for (size_t i = 0; i < table.size(); i++)
        {
            HashTableNode<T> *node = table[i];
            /* Move the whole chain, each element one by one. */
            while (node != nullptr) {
                HashTableNode<T> *next = node->next;
                size_t newIndex = hasher(node->data) % newTable.size();
                node->next = newTable[newIndex];
                newTable[newIndex] = node;
                node = next;
            }
        }

        /*
         * Set new table to storage.
         * Std::swap won't copy array, it's very cheap.
         */
        std::swap(table, newTable);
    }

    std::vector<HashTableNode<T> *> table;
    Hasher hasher;
    /* Amount of elemets in HT. */
    size_t size;
};

class PolyHash
{
public:
    PolyHash(size_t prime = 71): prime(prime) {}

    size_t operator()(const std::string &str)
    {
        /* Gorner's method. */
        size_t hash = 0;
        for (size_t i = 0; i < str.size(); i++) {
            hash = hash * prime + str[i];
        }
        return hash;
    }

private:
    size_t prime;
};

int
main()
{
    // HashTable<std::string, std::hash<std::string>> table;
    HashTable<std::string, PolyHash> table;

    /*
     * + K - Insert(K)
     * - K - Delete(K)
     * ? K - Has(K)
     */
    char op;
    std::string key;

    while (std::cin >> op >> key) {
        switch (op) {
        case '?':
            /* Ternary operator. */
            std::cout << (table.Has(key) ? "OK" : "FAIL") << std::endl;
            break;
        case '+':
            std::cout << (table.Add(key) ? "OK" : "FAIL") << std::endl;
            break;
        case '-':
            std::cout << (table.Delete(key) ? "OK" : "FAIL") << std::endl;
            break;
        }
    }
    return 0;
}
