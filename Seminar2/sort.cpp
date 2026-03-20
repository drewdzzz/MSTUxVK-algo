/**
 * This file contains implementation of non-template sort with comparators.
 * The sorting function support only `int` values, comparator are passed
 * as function pointers.
 */
#include <algorithm>
#include <iostream>

bool
comparator_less(int a, int b)
{
    return a < b;
}

bool
comparator_greater(int a, int b)
{
    return a > b;
}

/** `cmp` - comparator of elements. */
void
Sort(int *arr, int l, int r, bool (*cmp)(int, int))
{
    for (int i = l; i < r; i++) {
        for (int j = l; j < r; j++) {
            if (cmp(arr[j + 1], arr[j]))
                std::swap(arr[j], arr[j + 1]);
        }
    }
}

int
main()
{
    int arr[6] = {15, 7, 1, 92, 0, 2};
    Sort(arr, 0, 5, comparator_greater);
    for (int i = 0; i < 6; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
    return 0;
}
