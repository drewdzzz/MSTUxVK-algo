#include <iostream> /* Input Output. */
#include <sstream> /* Stringstream for test. */
#include <cassert> /* Assert for tests. */

/* Узел однозвязного списка. */
struct Node {
    Node(int value) : value(value), next(nullptr) {}
    int value;
    struct Node *next;
};

/* Очередь на односвязном списке. */
class Queue {
public: /* Interface. */
    Queue() : head(nullptr), tail(nullptr) {}

    /* Соблюдаем "правило трех". */
    Queue(const Queue &other) = delete;
    Queue operator=(const Queue &other) = delete;

    ~Queue()
    {
        while (!IsEmpty())
            Pop();
    }

    void
    Push(int value)
    {
        Node *node = new Node(value);
        if (IsEmpty()) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    int
    Pop()
    {
        if (IsEmpty())
            return -1;

        int value = head->value;
        Node *oldHead = head;

        head = head->next;
        if (head == nullptr)
            tail = nullptr;

        delete oldHead;
        return value;
    }

    bool
    IsEmpty()
    {
        return head == nullptr && tail == nullptr;
    }

private: /* Private data. */
    /* The oldest element. */
    Node *head;
    /* The newest element. */
    Node *tail;
};

/**
 * Решаем задачу.
 * in - поток ввода
 * out - поток вывода
 */
void
run(std::istream &in, std::ostream &out)
{
    Queue queue;

    int n = 0;
    in >> n;

    for (int i = 0; i < n; i++) {
        int op, value;
        in >> op >> value;

        switch (op) {
        case 2: {
            int returned = queue.Pop();
            if (returned != value) {
                out << "NO" << std::endl;
                return;
            }
            /* End of the case!!! */
            break;
        }
        case 3: {
            queue.Push(value);
            /* End of the case!!! */
            break;
        }
        }
    }
    out << "YES" << std::endl;
}

void
test()
{
    /* TESTCASE 1 */
    {
        std::stringstream in, out;
        in << "3" << std::endl;
        in << "3 44" << std::endl;
        in << "3 50" << std::endl;
        in << "2 44" << std::endl;
        run(in, out);
        assert(out.str() == "YES\n");
    }
    /* TESTCASE 2 */
    {
        std::stringstream in, out;
        in << "3" << std::endl;
        in << "3 44" << std::endl;
        in << "3 50" << std::endl;
        in << "2 46" << std::endl;
        run(in, out);
        assert(out.str() == "NO\n");
    }
}

int
main()
{
    // run(std::cin, std::cout);
    test();
    return 0;
}
