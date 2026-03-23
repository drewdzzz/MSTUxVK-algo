#include <iostream>

void
foo(int a = 5)
{
    std::cout << a << std::endl;
}

int main()
{
    foo(10);
    foo();
}
