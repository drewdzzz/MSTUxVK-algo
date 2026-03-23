#include <iostream>

/* Summ of all values. */
int
main()
{
    int sum = 0;
    int value;
    while (std::cin >> value)
        sum += value;

    std::cout << sum << std::endl;
}
