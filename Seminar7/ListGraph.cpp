#include "IGraph.hpp"

#include <deque>
#include <iostream>
#include <functional>
#include <vector>
#include "assert.h"

struct ListGraph: public IGraph
{
public:
    ListGraph(int size): data_(size) {}

    /**
     * Может принять любую реализацию графа.
     */
    ListGraph(const IGraph &graph) {
        /* Резеривирование (оптимизация). */
        data_.resize(graph.VerticesCount());

        for (int i = 0; i < graph.VerticesCount(); i++)
            data_[i] = graph.GetNextVertices(i);
    }

    ~ListGraph() {}

    void
    AddEdge(int from, int to) override {
        assert(0 <= from && from < data_.size());
        assert(0 <= to && to < data_.size());
        data_[from].push_back(to);
    }

    int
    VerticesCount() const override {
        return data_.size();
    }

    std::vector<int>
    GetNextVertices(int vertex) const override {
        return data_[vertex];
    }

    std::vector<int>
    GetPrevVertices(int vertex) const override {
        std::vector<int> prev;
        for (int from = 0; from < data_.size(); from++) {
           for (int to: data_[from]) {
               if (to == vertex)
                   prev.push_back(from);
           }
        }
        return prev;
    }

private:
    std::vector<std::vector<int>> data_;
};

/* Один запуск DFS, может обработать не все узлы. */
void
DFSImpl(const IGraph &graph, int vertex, std::vector<bool> &visited,
        const std::function<void(int)> &func)
{
    visited[vertex] = true;
    func(vertex);

    std::vector<int> next = graph.GetNextVertices(vertex);
    for (int nextVertex: next) {
        if (!visited[nextVertex])
            DFSImpl(graph, nextVertex, visited, func);
    }
}

void
DFS(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i])
            DFSImpl(graph, i, visited, func);
    }
}

void
topSortImpl(const IGraph &graph, int vertex, std::vector<bool> &visited, std::deque<int> &sorted)
{
    visited[vertex] = true;

    std::vector<int> next = graph.GetNextVertices(vertex);
    for (int nextVertex: next) {
        if (!visited[nextVertex])
            topSortImpl(graph, nextVertex, visited, sorted);
    }
    sorted.push_front(vertex);
}

std::deque<int>
topSort(const IGraph &graph)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::deque<int> sorted;

    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i])
            topSortImpl(graph, i, visited, sorted);
    }

    return sorted;
}

int
main()
{
    ListGraph graph(7);
    graph.AddEdge(0, 1);
    // graph.AddEdge(0, 5);
    graph.AddEdge(5, 0);
    graph.AddEdge(1, 6);
    graph.AddEdge(5, 6);
    graph.AddEdge(5, 4);

    DFS(graph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    std::deque<int> sorted = topSort(graph);
    for (int v : sorted)
        std::cout << v << " ";
    std::cout << std::endl;
    return 0;
}
