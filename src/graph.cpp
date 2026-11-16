
#include "graph.h"

// ==========================================
// CUSTOM MIN HEAP
// ==========================================

MinHeap::MinHeap(int size)
{
    capacity = size;
    heapSize = 0;

    arr = new MinHeapNode[capacity];
}

void MinHeap::swapNodes(int i, int j)
{
    MinHeapNode temp = arr[i];

    arr[i] = arr[j];
    arr[j] = temp;
}

// Time Complexity: O(log n)
void MinHeap::heapifyUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (arr[parent].distance <= arr[index].distance)
            break;

        swapNodes(parent, index);

        index = parent;
    }
}

// Time Complexity: O(log n)
void MinHeap::heapifyDown(int index)
{
    while (true)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        int smallest = index;

        if (left < heapSize &&
            arr[left].distance < arr[smallest].distance)
        {
            smallest = left;
        }

        if (right < heapSize &&
            arr[right].distance < arr[smallest].distance)
        {
            smallest = right;
        }

        if (smallest == index)
            break;

        swapNodes(index, smallest);

        index = smallest;
    }
}

// Time Complexity: O(log n)
void MinHeap::insert(int vertex, double distance)
{
    if (heapSize == capacity)
        return;

    arr[heapSize].vertex = vertex;
    arr[heapSize].distance = distance;

    heapifyUp(heapSize);

    heapSize++;
}

// Time Complexity: O(log n)
MinHeapNode MinHeap::extractMin()
{
    if (heapSize == 0)
        return {-1, INF};

    MinHeapNode result = arr[0];

    heapSize--;

    if (heapSize > 0)
    {
        arr[0] = arr[heapSize];

        heapifyDown(0);
    }

    return result;
}

// Time Complexity: O(1)
bool MinHeap::isEmpty() const
{
    return heapSize == 0;
}

// Decrease Key
// Time Complexity: O(n) for locating vertex + O(log n) for heapify.
void MinHeap::decreaseKey(int vertex, double distance)
{
    for (int i = 0; i < heapSize; i++)
    {
        if (arr[i].vertex == vertex)
        {
            if (distance < arr[i].distance)
            {
                arr[i].distance = distance;

                heapifyUp(i);
            }

            return;
        }
    }
}

MinHeap::~MinHeap()
{
    delete[] arr;
}

// ==========================================
// GRAPH IMPLEMENTATION
// ==========================================

// Constructor
// Time Complexity: O(V)
Graph::Graph(int n)
{
    vertices = n;

    if (vertices > MAX_GRAPH_NODES)
        vertices = MAX_GRAPH_NODES;

    if (vertices < 1)
        vertices = 1;

    for (int i = 0; i < MAX_GRAPH_NODES; i++)
        adjacency[i] = NULL;
}

// Add Node
// Time Complexity: O(1)
void Graph::addNode()
{
    if (vertices >= MAX_GRAPH_NODES)
        return;

    adjacency[vertices] = NULL;

    vertices++;
}

// Add Edge
// Undirected graph.
// Time Complexity: O(1)
void Graph::addEdge(int u, int v, double weight)
{
    if (u < 0 || v < 0 ||
        u >= vertices || v >= vertices ||
        u == v || weight <= 0)
    {
        return;
    }

    // Check for an existing edge.
    Edge* temp = adjacency[u];

    while (temp != NULL)
    {
        if (temp->destination == v)
            return;

        temp = temp->next;
    }

    Edge* first = new Edge(v, weight);

    first->next = adjacency[u];

    adjacency[u] = first;

    Edge* second = new Edge(u, weight);

    second->next = adjacency[v];

    adjacency[v] = second;
}

// Display Graph
// Time Complexity: O(V + E)
void Graph::display() const
{
    cout << "\n========== GRAPH ==========\n";

    for (int i = 0; i < vertices; i++)
    {
        cout << "Node " << i << " : ";

        Edge* temp = adjacency[i];

        while (temp != NULL)
        {
            cout << temp->destination
                 << "(" << temp->weight << " km) ";

            temp = temp->next;
        }

        cout << endl;
    }
}

// Get Vertices
// Time Complexity: O(1)
int Graph::getVertices() const
{
    return vertices;
}

// ==========================================
// DIJKSTRA
// ==========================================

// Time Complexity:
// O((V + E) log V) for a proper decrease-key min-heap.
// This implementation uses a linear scan to find a heap node
// during decreaseKey, so its actual bound is O(V^2 + E log V).
void Graph::dijkstraInternal(
    int source,
    double distance[],
    int parent[]
)
{
    bool visited[MAX_GRAPH_NODES] = {false};

    for (int i = 0; i < vertices; i++)
    {
        distance[i] = INF;
        parent[i] = -1;
    }

    distance[source] = 0;

    MinHeap heap(vertices * vertices + 1);

    heap.insert(source, 0);

    while (!heap.isEmpty())
    {
        MinHeapNode current = heap.extractMin();

        int u = current.vertex;

        if (u == -1)
            break;

        if (visited[u])
            continue;

        visited[u] = true;

        Edge* temp = adjacency[u];

        while (temp != NULL)
        {
            int v = temp->destination;

            double weight = temp->weight;

            if (!visited[v] &&
                distance[u] + weight < distance[v])
            {
                distance[v] = distance[u] + weight;

                parent[v] = u;

                // Insert updated distance.
                heap.insert(v, distance[v]);
            }

            temp = temp->next;
        }
    }
}

// Public Dijkstra function.
// Time Complexity: O(V^2 + E log V) with this heap implementation.
void Graph::dijkstra(
    int source,
    double distance[],
    int parent[]
)
{
    if (source < 0 || source >= vertices)
        return;

    dijkstraInternal(source, distance, parent);
}

// ==========================================
// SHORTEST PATH RECONSTRUCTION
// ==========================================

// Time Complexity: O(V)
int Graph::getPath(
    int source,
    int destination,
    int parent[],
    int path[]
)
{
    int reversePath[MAX_GRAPH_NODES];

    int count = 0;

    int current = destination;

    while (current != -1 && count < MAX_GRAPH_NODES)
    {
        reversePath[count++] = current;

        if (current == source)
            break;

        current = parent[current];
    }

    if (count == 0 || reversePath[count - 1] != source)
        return 0;

    // Reverse path to get source -> destination.
    for (int i = 0; i < count; i++)
        path[i] = reversePath[count - 1 - i];

    return count;
}

// ==========================================
// MULTI-BIN ROUTE PLANNING
// ==========================================

// Nearest Neighbour heuristic.
// Time Complexity: O(N * V^2 + N^2)
// Route reconstruction adds O(NV) in the worst case.
RouteResult Graph::planRoute(
    int urgentBins[],
    int n,
    int depot
)
{
    RouteResult result;

    if (n <= 0 || n >= MAX_GRAPH_NODES ||
        depot < 0 || depot >= vertices)
    {
        return result;
    }

    bool selected[MAX_GRAPH_NODES] = {false};

    int current = depot;

    double distance[MAX_GRAPH_NODES];

    int parent[MAX_GRAPH_NODES];

    // Start route from depot.
    result.visitOrder[result.visitCount++] = depot;

    for (int step = 0; step < n; step++)
    {
        dijkstraInternal(current, distance, parent);

        int nearest = -1;

        double minimum = INF;

        // Find nearest unvisited urgent bin.
        for (int i = 0; i < n; i++)
        {
            int bin = urgentBins[i];

            if (bin < 0 || bin >= vertices ||
                bin == depot || selected[bin])
            {
                continue;
            }

            if (distance[bin] < minimum)
            {
                minimum = distance[bin];

                nearest = bin;
            }
        }

        // No reachable urgent bin.
        if (nearest == -1 || minimum == INF)
            break;

        selected[nearest] = true;

        // Save visit order.
        result.visitOrder[result.visitCount++] = nearest;

        result.legDistances[step] = minimum;

        result.totalDistance += minimum;

        // Reconstruct full shortest path.
        int path[MAX_GRAPH_NODES];

        int pathLength = getPath(
            current,
            nearest,
            parent,
            path
        );

        if (pathLength == 0)
            break;

        // Append path without repeating its first node.
        for (int i = 0; i < pathLength; i++)
        {
            if (result.pathCount == 0 ||
                result.fullPath[result.pathCount - 1] != path[i])
            {
                if (result.pathCount <
                    MAX_GRAPH_NODES * MAX_GRAPH_NODES)
                {
                    result.fullPath[result.pathCount++] = path[i];
                }
            }
        }

        current = nearest;
    }

    return result;
}

// Destructor
// Time Complexity: O(V + E)
Graph::~Graph()
{
    for (int i = 0; i < vertices; i++)
    {
        Edge* temp = adjacency[i];

        while (temp != NULL)
        {
            Edge* nextNode = temp->next;

            delete temp;

            temp = nextNode;
        }
    }
}
