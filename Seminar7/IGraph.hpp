#pragma once
/* Хидер вставится строго один раз. */

#include <vector>

struct IGraph {
    virtual ~IGraph() {}

	// Добавление ребра от from к to.
	virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const  = 0;

	/* Получить всех соседей. */
	virtual std::vector<int> GetNextVertices(int vertex) const = 0;
	/* Получить все вершины, для которых мы - сосед. */
	virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};
