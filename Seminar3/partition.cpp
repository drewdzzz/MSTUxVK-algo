#include <functional> /* for std::less */
#include <iostream>
#include <vector>

/** std::less - standard comparator for `<`. */
template <typename T, typename Comparator = std::less<T>>
int
partition(T *arr, int l, int r, Comparator cmp = Comparator())
/** `Comparator cmp = Comparator()` create Comparator object with default constructor and set it default argument. */
{
    T pivot = arr[l]; /* HomeTask: better strategy. */
    int i = l + 1; /* Left iterator, goes forward. */
    int j = r; /* Right iterator, goes backwards. */

    while (true) {
        /* Skip elements where a[i] < p */
        while (i <= r && cmp(arr[i], pivot))
            i++;
        /* Skip elements where a[i] >= p */
        while (cmp(pivot, arr[j]))
            j--;
        /* [p, ... < p ...](i) - i overflow */
        /* [p(j), ... >= p ...] -j **no** overflow */

        if (i >= j) {
            /* arr[j] < arr[j] (which is `pivot`) */
            std::swap(arr[l], arr[j]);
            return j; /* p is arr[j] now */
        }

        std::swap(arr[i++], arr[j--]);
        /* The same as:
         * std::swap(arr[i], arr[j]);
         * i++;
         * j--;
         */
    }
}

template <typename T, typename Comparator = std::less<T>>
T
kth_statistics(T *arr, int k, int l, int r, Comparator cmp = Comparator())
{
    int pivot_pos = partition(arr, l, r, cmp);

    if (pivot_pos == k) {
        return arr[pivot_pos];
    }
    /* Recursion is bad! Only if sure depth is not large. */
    if (pivot_pos > k) {
        return kth_statistics(arr, k, l, pivot_pos - 1, cmp);
    }
    /* pivot_pos < k */
    return kth_statistics(arr, k, pivot_pos + 1, r, cmp);
}

/**
 * `std::less(a, b)` - true if `a < b`, false if `a >= b`.
 * `std::greater(a, b)` - true if `a > b`, false if `a <= b`.
 */
void
basic_test()
{
    std::vector<int> array;
    array.push_back(10);
    array.push_back(1);
    array.push_back(5);
    array.push_back(9);
    array.push_back(7);

    /* Kth statistics with `less`. */
    for (int i = 0; i < array.size(); i++) {
        std::cout << kth_statistics(array.data(), i, 0, array.size() - 1) << " ";
    }
    std::cout << std::endl;

    /* Kth statistics with `greater`. */
    for (int i = 0; i < array.size(); i++) {
        std::cout << kth_statistics(array.data(), i, 0, array.size() - 1, std::greater<int>()) << " ";
        /*
         * Another variant:
         * std::cout << kth_statistics<int, std::greater<int>>(array.data(), i, 0, array.size() - 1) << " ";
         */
    }
    std::cout << std::endl;
}

/* It must crash because of deep recursion. */
void
huge_sorted_test()
{
    std::vector<int> array;
    for (int i = 0; i < 1000000; i++)
        array.push_back(i);
    std::cout << kth_statistics(array.data(), array.size() - 1, 0, array.size() - 1) << std::endl;
}

int
main()
{
    basic_test();
    /* huge_sorted_test(); */
    return 0;
}
