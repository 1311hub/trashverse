
#include "heap.h"

// Constructor
MaxHeap::MaxHeap(int size)
{
    capacity = size;
    heapSize = 0;

    arr = new HeapNode[capacity];

    for (int i = 0; i < MAX_BIN_ID; i++)
        position[i] = -1;
}

// Swap two heap nodes.
// Time Complexity: O(1)
void MaxHeap::swapNodes(int i, int j)
{
    HeapNode temp = arr[i];

    arr[i] = arr[j];
    arr[j] = temp;

    position[arr[i].bin_id] = i;
    position[arr[j].bin_id] = j;
}

// Move node upward.
// Time Complexity: O(log n)
void MaxHeap::heapifyUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (arr[parent].fill_level >= arr[index].fill_level)
            break;

        swapNodes(parent, index);

        index = parent;
    }
}

// Move node downward.
// Time Complexity: O(log n)
void MaxHeap::heapifyDown(int index)
{
    while (true)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        int largest = index;

        if (left < heapSize &&
            arr[left].fill_level > arr[largest].fill_level)
        {
            largest = left;
        }

        if (right < heapSize &&
            arr[right].fill_level > arr[largest].fill_level)
        {
            largest = right;
        }

        if (largest == index)
            break;

        swapNodes(index, largest);

        index = largest;
    }
}

// Constructor
// Time Complexity: O(1)
MaxHeap::MaxHeap(int size);

// Destructor
MaxHeap::~MaxHeap()
{
    delete[] arr;
}

// Insert
// Time Complexity: O(log n)
void MaxHeap::insert(int bin_id, int fill_level)
{
    if (bin_id < 0 || bin_id >= MAX_BIN_ID)
    {
        cout << "Invalid bin ID.\n";
        return;
    }

    if (heapSize == capacity)
    {
        cout << "Heap is full.\n";
        return;
    }

    if (position[bin_id] != -1)
    {
        updateKey(bin_id, fill_level);
        return;
    }

    int index = heapSize;

    arr[index].bin_id = bin_id;
    arr[index].fill_level = fill_level;

    position[bin_id] = index;

    heapSize++;

    heapifyUp(index);
}

// Peek
// Time Complexity: O(1)
HeapNode MaxHeap::peek() const
{
    if (heapSize == 0)
    {
        cout << "Heap is empty.\n";

        return {-1, -1};
    }

    return arr[0];
}

// Extract Maximum
// Time Complexity: O(log n)
HeapNode MaxHeap::extractMax()
{
    if (heapSize == 0)
    {
        cout << "Heap is empty.\n";

        return {-1, -1};
    }

    HeapNode maximum = arr[0];

    position[maximum.bin_id] = -1;

    heapSize--;

    if (heapSize > 0)
    {
        arr[0] = arr[heapSize];

        position[arr[0].bin_id] = 0;

        heapifyDown(0);
    }

    return maximum;
}

// Increase Key
// Time Complexity: O(log n)
bool MaxHeap::increaseKey(int bin_id, int newFill)
{
    if (bin_id < 0 || bin_id >= MAX_BIN_ID)
        return false;

    int index = position[bin_id];

    if (index == -1)
        return false;

    if (newFill < arr[index].fill_level)
        return false;

    arr[index].fill_level = newFill;

    heapifyUp(index);

    return true;
}

// Update Key
// Time Complexity: O(log n)
bool MaxHeap::updateKey(int bin_id, int newFill)
{
    if (bin_id < 0 || bin_id >= MAX_BIN_ID)
        return false;

    int index = position[bin_id];

    if (index == -1)
        return false;

    int oldFill = arr[index].fill_level;

    arr[index].fill_level = newFill;

    if (newFill > oldFill)
        heapifyUp(index);
    else
        heapifyDown(index);

    return true;
}

// Build Heap
// Time Complexity: O(n)
void MaxHeap::buildHeap(HeapNode nodes[], int n)
{
    if (n > capacity)
    {
        cout << "Not enough heap capacity.\n";
        return;
    }

    for (int i = 0; i < MAX_BIN_ID; i++)
        position[i] = -1;

    heapSize = 0;

    for (int i = 0; i < n; i++)
    {
        if (nodes[i].bin_id < 0 ||
            nodes[i].bin_id >= MAX_BIN_ID ||
            position[nodes[i].bin_id] != -1)
        {
            cout << "Invalid or duplicate bin ID.\n";
            heapSize = 0;

            for (int j = 0; j < MAX_BIN_ID; j++)
                position[j] = -1;

            return;
        }

        arr[i] = nodes[i];

        position[nodes[i].bin_id] = i;

        heapSize++;
    }

    for (int i = heapSize / 2 - 1; i >= 0; i--)
        heapifyDown(i);
}

// Size
// Time Complexity: O(1)
int MaxHeap::size() const
{
    return heapSize;
}

// Is Empty
// Time Complexity: O(1)
bool MaxHeap::isEmpty() const
{
    return heapSize == 0;
}

// Is Full
// Time Complexity: O(1)
bool MaxHeap::isFull() const
{
    return heapSize == capacity;
}

// Display
// Time Complexity: O(n)
void MaxHeap::display() const
{
    cout << "\n========== MAX HEAP ==========\n";

    for (int i = 0; i < heapSize; i++)
    {
        cout << "Bin ID: " << arr[i].bin_id
             << "  Fill: " << arr[i].fill_level
             << "%\n";
    }
}
