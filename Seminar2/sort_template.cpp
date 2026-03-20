/**
 * This file contains various implementations of template sort.
 */
#include <algorithm>
#include <iostream>

/* Point structure. */
struct Point {
    int x;
    int y;
};

/* Operator "less" for point. */
bool operator<(const Point &left, const Point &right)
{
    return left.x < right.x;
}

/** Comparators. */
bool
CmpInt(const int &a, const int &b)
{
    return a < b;
}

bool
CmpPointX(const Point &a, const Point &b)
{
    return a.x < b.x;
}

bool
CmpPointY(const Point &a, const Point &b)
{
    return a.y < b.y;
}

bool
CmpPointSum(const Point &a, const Point &b)
{
    return a.y + a.x < b.y + b.x;
}

/** Sort with comparator as function pointer. */
template <class T>
void
Sort(T *arr, int l, int r, bool (*cmp)(const T&, const T&))
{
    for (int i = l; i < r; i++) {
        for (int j = l; j < r; j++) {
            if (cmp(arr[j + 1], arr[j]))
                std::swap(arr[j], arr[j + 1]);
        }
    }
}

/** Sort with operator "less" - the type must have it. */
template <class T>
void
SortByOperator(T *arr, int l, int r)
{
    for (int i = l; i < r; i++) {
        for (int j = l; j < r; j++) {
            if (arr[j + 1] < arr[j])
                std::swap(arr[j], arr[j + 1]);
        }
    }
}

/* Comparator class, allows to save state. */
class PointComparatorDist {
public:
    PointComparatorDist(const Point &p0) : point(p0) {}

    /* Теперь можно вызывать как функцию. */
    bool operator()(const Point &a, const Point &b)
    {
        double distance_a = std::sqrt((point.x - a.x) * (point.x - a.x) + (point.x - a.y) * (point.x - a.y));
        double distance_b = std::sqrt((point.x - b.x) * (point.x - b.x) + (point.x - b.y) * (point.x - b.y));
        return distance_a < distance_b;
    }
private:
    Point point;
};

/** Sort with functor. */
template <class T, class Comparator>
void
SortByFunctor(T *arr, int l, int r, Comparator cmp)
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
    /* Sort with comparator as function pointer. */
    int arr[6] = {15, 7, 1, 92, 0, 2};
    Sort<int>(arr, 0, 5, CmpInt);
    for (int i = 0; i < 6; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;

    Point points[6];
    for (int i = 0; i < 6; i++) {
        points[i] = {i % 2, i % 3};
    }

    Sort<Point>(points, 0, 5, CmpPointX); /* Sort by X coordinate. */
    for (int i = 0; i < 6; i++)
        std::cout << points[i].x << ":" << points[i].y << " ";
    std::cout << std::endl;

    /* Sort<int>(points, 0, 5, CmpPointY); - won't work because of type mismatch. */
    Sort(points, 0, 5, CmpPointY); /* Sort by X coordinate. */
    for (int i = 0; i < 6; i++)
        std::cout << points[i].x << ":" << points[i].y << " ";
    std::cout << std::endl;

    Sort(points, 0, 5, CmpPointSum); /* Sort by coordinates sum. */
    for (int i = 0; i < 6; i++)
        std::cout << points[i].x << ":" << points[i].y << " ";
    std::cout << std::endl;

    /* Sort with operator "less" (hardcoded in the function). */
    std::swap(arr[0], arr[4]); /* Make array unsorted. */
    SortByOperator(arr, 0, 5);
    for (int i = 0; i < 6; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;

    SortByOperator(points, 0, 5);
    for (int i = 0; i < 6; i++)
        std::cout << points[i].x << ":" << points[i].y << " ";
    std::cout << std::endl;

    /* Sort with functor - the best approach!!! */
    std::cout << "SORT BY Y WITH FUNCTOR (ptr to function)" << std::endl;
    SortByFunctor(points, 0, 5, CmpPointY);
    for (int i = 0; i < 6; i++)
        std::cout << points[i].x << ":" << points[i].y << " ";
    std::cout << std::endl;

    std::cout << "SORT BY DISTANCE FROM {0, 0} WITH FUNCTOR" << std::endl;
    SortByFunctor(points, 0, 5, PointComparatorDist({0, 0}));
    for (int i = 0; i < 6; i++)
        std::cout << points[i].x << ":" << points[i].y << " ";
    std::cout << std::endl;

    std::cout << "SORT BY DISTANCE FROM {5, 5} WITH FUNCTOR" << std::endl;
    SortByFunctor(points, 0, 5, PointComparatorDist({5, 5}));
    for (int i = 0; i < 6; i++)
        std::cout << points[i].x << ":" << points[i].y << " ";
    std::cout << std::endl;

    return 0;
}
