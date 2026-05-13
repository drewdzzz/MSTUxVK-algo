#include <string>
#include <iostream>

/*
struct IGraph {
virtual ~IGraph() {}

    // Добавление ребра от from к to.
	virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const  = 0;

	virtual std::vector<int> GetNextVertices(int vertex) const = 0;
	virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};
*/

class Animal {
public:
    virtual std::string Say() const {
        return "<Silence>";
    }
};

class Dog: public Animal {
public:
    std::string Say() const override {
        return "Woof";
    }
};

class Cat: public Animal {
public:
    std::string Say() const override {
        return "Meow";
    }
};

void
print(const Animal &animal)
{
    std::cout << animal.Say() << std::endl;
}

int
main()
{
    Animal animal;
    Dog dog;
    Cat cat;

    print(animal);
    print(dog);
    print(cat);

    return 0;
}
