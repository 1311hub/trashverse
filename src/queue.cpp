
#include "queue.h"
#include <cstring>

// Linked Queue Constructor
// Time Complexity: O(1)
LinkedQueue::LinkedQueue()
{
    front = NULL;
    rear = NULL;
    count = 0;
}

// Enqueue
// Time Complexity: O(1)
void LinkedQueue::enqueue(int bin_id)
{
    QueueNode* newNode = new QueueNode(bin_id);

    if (rear == NULL)
    {
        front = rear = newNode;
    }
    else
    {
        rear->next = newNode;
        rear = newNode;
    }

    count++;
}

// Dequeue
// Time Complexity: O(1)
int LinkedQueue::dequeue()
{
    if (front == NULL)
    {
        cout << "Queue is empty.\n";
        return -1;
    }

    QueueNode* temp = front;

    int id = temp->bin_id;

    front = front->next;

    if (front == NULL)
        rear = NULL;

    delete temp;

    count--;

    return id;
}

// Peek
// Time Complexity: O(1)
int LinkedQueue::peek() const
{
    if (front == NULL)
        return -1;

    return front->bin_id;
}

// Is Empty
// Time Complexity: O(1)
bool LinkedQueue::isEmpty() const
{
    return front == NULL;
}

// Size
// Time Complexity: O(1)
int LinkedQueue::size() const
{
    return count;
}

// Display
// Time Complexity: O(n)
void LinkedQueue::display() const
{
    QueueNode* temp = front;

    cout << "\nCollection Queue: ";

    while (temp != NULL)
    {
        cout << "B" << temp->bin_id;

        if (temp->next != NULL)
            cout << " -> ";

        temp = temp->next;
    }

    cout << endl;
}

// Clear
// Time Complexity: O(n)
void LinkedQueue::clear()
{
    while (front != NULL)
        dequeue();
}

// Destructor
LinkedQueue::~LinkedQueue()
{
    clear();
}

// Circular Queue Constructor
// Time Complexity: O(n)
CircularQueue::CircularQueue(int size)
{
    capacity = size;

    shifts = new char[capacity * 20];

    front = 0;
    rear = -1;
    count = 0;

    for (int i = 0; i < capacity * 20; i++)
        shifts[i] = '\0';
}

// Circular Queue Enqueue
// Time Complexity: O(1)
void CircularQueue::enqueue(const char shift[])
{
    if (isFull())
    {
        cout << "Circular queue is full.\n";
        return;
    }

    rear = (rear + 1) % capacity;

    strcpy(shifts + rear * 20, shift);

    count++;
}

// Circular Queue Dequeue
// Time Complexity: O(1)
void CircularQueue::dequeue(char result[])
{
    if (isEmpty())
    {
        strcpy(result, "None");
        return;
    }

    strcpy(result, shifts + front * 20);

    shifts[front * 20] = '\0';

    front = (front + 1) % capacity;

    count--;

    if (count == 0)
    {
        front = 0;
        rear = -1;
    }
}

// Is Empty
// Time Complexity: O(1)
bool CircularQueue::isEmpty() const
{
    return count == 0;
}

// Is Full
// Time Complexity: O(1)
bool CircularQueue::isFull() const
{
    return count == capacity;
}

// Size
// Time Complexity: O(1)
int CircularQueue::size() const
{
    return count;
}

// Display
// Time Complexity: O(n)
void CircularQueue::display() const
{
    cout << "\nShift Schedule: ";

    for (int i = 0; i < count; i++)
    {
        int index = (front + i) % capacity;

        cout << shifts + index * 20 << " ";
    }

    cout << endl;
}

// Destructor
CircularQueue::~CircularQueue()
{
    delete[] shifts;
}
