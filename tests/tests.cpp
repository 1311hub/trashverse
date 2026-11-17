
#include <iostream>
#include <cassert>
#include "hashmap.h"
#include "heap.h"
#include "queue.h"
#include "graph.h"
#include "ledger.h"

using namespace std;

void testHashMap()
{
    HashMap map(10);

    BinRecord b;
    b.id = 1;
    b.fill_level = 75;

    assert(map.insert(b));
    assert(map.size() == 1);
    assert(map.contains(1));
    assert(map.get(1)->fill_level == 75);

    b.fill_level = 90;
    assert(map.update(b));
    assert(map.get(1)->fill_level == 90);

    assert(map.remove(1));
    assert(!map.contains(1));
}

void testMaxHeap()
{
    MaxHeap heap(10);

    heap.insert(1, 40);
    heap.insert(2, 90);
    heap.insert(3, 70);

    assert(heap.size() == 3);
    assert(heap.peek().bin_id == 2);
    assert(heap.extractMax().fill_level == 90);
    assert(heap.extractMax().fill_level == 70);
}

void testLinkedQueue()
{
    LinkedQueue queue;

    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    assert(queue.size() == 3);
    assert(queue.peek() == 10);
    assert(queue.dequeue() == 10);
    assert(queue.dequeue() == 20);
    assert(queue.dequeue() == 30);
    assert(queue.isEmpty());
}

void testGraph()
{
    Graph graph(4);

    graph.addEdge(0, 1, 2);
    graph.addEdge(1, 2, 3);
    graph.addEdge(2, 3, 4);
    graph.addEdge(0, 3, 15);

    double distance[100];
    int parent[100];

    graph.dijkstra(0, distance, parent);

    assert(distance[0] == 0);
    assert(distance[1] == 2);
    assert(distance[2] == 5);
    assert(distance[3] == 9);

    int path[100];
    int count = graph.getPath(0, 3, parent, path);

    assert(count == 4);
    assert(path[0] == 0);
    assert(path[3] == 3);
}

void testLedger()
{
    WorkerLedger ledger(30, 200);

    assert(ledger.addWorker(1, "TestWorker", 500));
    assert(ledger.recordCollection(1, 20, 5));

    assert(ledger.calculatePay(1) == 700);
    assert(ledger.getWorkerCount() == 1);
}

int main()
{
    testHashMap();
    testMaxHeap();
    testLinkedQueue();
    testGraph();
    testLedger();

    cout << "All tests passed successfully.\n";

    return 0;
}
