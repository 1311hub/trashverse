
#ifndef HEAP_H
#define HEAP_H

#include <iostream>
using namespace std;

#define MAX_BIN_ID 10000

struct HeapNode
{
    int bin_id;
    int fill_level;
};

class MaxHeap
{
private:

    HeapNode* arr;

    // Position map: bin ID -> heap array index.
    int position[MAX_BIN_ID];

    int capacity;
    int heapSize;

    void swapNodes(int i, int j);

    void heapifyUp(int index);

    void heapifyDown(int index);

public:

    MaxHeap(int size = 100);

    ~MaxHeap();

    // Insert a bin.
    void insert(int bin_id, int fill_level);

    // Remove highest-priority bin.
    HeapNode extractMax();

    // Return highest-priority bin without removing it.
    HeapNode peek() const;

    // Increase a bin's fill level.
    bool increaseKey(int bin_id, int newFill);

    // Update fill level in either direction.
    bool updateKey(int bin_id, int newFill);

    // Build heap from an array.
    void buildHeap(HeapNode nodes[], int n);

    int size() const;

    bool isEmpty() const;

    bool isFull() const;

    void display() const;
};

#endif
