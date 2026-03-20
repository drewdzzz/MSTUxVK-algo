/**
 * The file introduces `const` and `explicit` keywords.
 */
#include <iostream>

class Class {
public:
    /* Forbids implicit conversion. */
    explicit Class(int value) : value(value) {}
    int GetValue() const /* Available in const objects. */
    {
        /* value = 5; - cannot change. */
        return value;
    }
private:
    int value;
};

int
main()
{
    int a = 10;
    /* Immutable. */
    const int b = 10;
    a = 12;
    // b = 12; - cannot change.
    const Class obj(10);
    std::cout << obj.GetValue() << std::endl;
    return 0;
}
