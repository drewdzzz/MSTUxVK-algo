#include <iostream>

void
CountingSortNonStable(int *arr, int n)
{
    int minVal = arr[0];
    int maxVal = arr[0];

    for (int i = 1; i < n; i++) {
        minVal = std::min(minVal, arr[i]);
        maxVal = std::max(maxVal, arr[i]);
    }

    /*
     * Create and initialize count buffer.
     * For [minVal, maxVal] range only.
     */
    int countBufSize = maxVal - minVal + 1;
    int *countBuf = new int[countBufSize];
    for (int i = 0; i < countBufSize; i++)
        countBuf[i] = 0;

    for (int i = 0; i < n; i++)
        countBuf[arr[i] - minVal]++;

    int cursor = 0;
    for (int i = 0; i < countBufSize; i++) {
        for (int j = 0; j < countBuf[i]; j++) {
            arr[cursor++] = i + minVal;
        }
    }

    delete[] countBuf;
}

void
CountingSortStable(int *arr, int n)
{
    int minVal = arr[0];
    int maxVal = arr[0];

    for (int i = 1; i < n; i++) {
        minVal = std::min(minVal, arr[i]);
        maxVal = std::max(maxVal, arr[i]);
    }

    /*
     * Create and initialize count buffer.
     * For [minVal, maxVal] range only.
     */
    int countBufSize = maxVal - minVal + 1;
    int *countBuf = new int[countBufSize];
    for (int i = 0; i < countBufSize; i++)
        countBuf[i] = 0;

    for (int i = 0; i < n; i++)
        countBuf[arr[i] - minVal]++;

    /* Temporary array for sorted data. */
    int *sorted = new int[n];

    /*
     * Calculate prefix sum. Each element is end position for `minVal + i` element.
     */
    for (int i = 1; i < countBufSize; i++)
        countBuf[i] += countBuf[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        /* Turn element into pos in `countBuf`. */
        int countBufPos = arr[i] - minVal;
        /* Advance cursor before operation to turn into 0-indexation from 1-indexation. */
        countBuf[countBufPos]--;
        /* Put element into sorted array. */
        sorted[countBuf[countBufPos]] = arr[i];
    }

    for (int i = 0; i < n; i++)
        arr[i] = sorted[i];

    delete[] countBuf;
    delete[] sorted;
}

int
main()
{
    std::vector<int> array;
    array.push_back(10);
    array.push_back(1);
    array.push_back(5);
    array.push_back(9);
    array.push_back(7);

    CountingSortStable(array.data(), array.size());
    for (int i = 0; i < array.size(); i++)
        std::cout << array[i] << " ";
    std::cout << std::endl;
}
