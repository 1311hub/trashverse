
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
using namespace std;

#define MAX_GRAPH_NODES 100
#define INF 1000000000.0

// Adjacency list node.
struct Edge
{
    int destination;
    double weight;

    Edge* next;

    Edge(int d, double w)
    {
        destination = d;
        weight = w;
        next = NULL;
    }
};

// Node used by our custom MIN heap.
struct MinHeapNode
{
    int vertex;
    double distance;
};

// Route information.
struct RouteResult
{
    int visitOrder[MAX_GRAPH_NODES];
    int visitCount;

    int fullPath[MAX_GRAPH_NODES * MAX_GRAPH_NODES];
    int pathCount;

    double legDistances[MAX_GRAPH_NODES];
    double totalDistance;

    RouteResult()
    {
        visitCount = 0;
        pathCount = 0;
        totalDistance = 0;

        for (int i = 0; i < MAX_GRAPH_NODES; i++)
        {
            visitOrder[i] = -1;
            legDistances[i] = 0;
        }
    }
};

// Custom MIN HEAP for Dijkstra.
class MinHeap
{
private:

    MinHeapNode* arr;

    int heapSize;
    int capacity;

    void swapNodes(int i, int j);

    void heapifyUp(int index);

    void heapifyDown(int index);

public:

    MinHeap(int size);

    ~MinHeap();

    void insert(int vertex, double distance);

    MinHeapNode extractMin();

    bool isEmpty() const;

    void decreaseKey(int vertex, double distance);
};

class Graph
{
private:

    int vertices;

    Edge* adjacency[MAX_GRAPH_NODES];

    void dijkstraInternal(
        int source,
        double distance[],
        int parent[]
    );

public:

    Graph(int n = MAX_GRAPH_NODES);

    ~Graph();

    void addNode();

    void addEdge(int u, int v, double weight);

    void display() const;

    int getVertices() const;

    // Dijkstra's shortest path algorithm.
    void dijkstra(
        int source,
        double distance[],
        int parent[]
    );

    // Reconstruct shortest path.
    int getPath(
        int source,
        int destination,
        int parent[],
        int path[]
    );

    // Plan route for multiple urgent bins.
    RouteResult planRoute(
        int urgentBins[],
        int n,
        int depot = 0
    );
};

#endif
