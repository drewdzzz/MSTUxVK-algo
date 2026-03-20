/**
 * The file contains interface example for binary heap class.
 */
#include <vector>

/**
 * Binary heap for `int` values.
 */
class Heap {
public: /* Public interface, visible to user. */

    /* Turn array into heap in O(n). */
    explicit Heap(const std::vector<int> &array);
    Heap();
    ~Heap();
    /* Don't forget rule of three. */

    /* Operations. */
    void Insert(int element);
    int RemoveMin();
    int GetMin() const;
private: /* Private state and methods, **not** visible to user. */

    /* Helpers for maintaining heap invariant. */
    void siftDown(int index);
    void siftUp(int index);

    /* The heap itself. */
    std::vector<int> data;
};

int
main()
{
    Heap heap;
    return 0;
}
