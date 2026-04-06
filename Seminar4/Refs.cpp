/** Const and non-const references. */
#include <iostream>

/** Only lvalues (variables). */
void
foo(int &a)
{
    std::cout << a << std::endl;
}

/** Both lvalues and rvalues (variables and temporary values). */
void
bar(const int &a)
{
    std::cout << a << std::endl;
}

int
main()
{
    int a = 10;
    foo(a);
    /* foo(10); - не компилируется, нельзя передать по ссылке временное значение. */

    bar(a);
    bar(10);
}
