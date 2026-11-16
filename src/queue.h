
#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
using namespace std;

// Node of linked-list queue.
struct QueueNode
{
    int bin_id;
    QueueNode* next;

    QueueNode(int id)
    {
        bin_id = id;
        next = NULL;
    }
};

class LinkedQueue
{
private:

    QueueNode* front;
    QueueNode* rear;

    int count;

public:

    LinkedQueue();

    ~LinkedQueue();

    void enqueue(int bin_id);

    int dequeue();

    int peek() const;

    bool isEmpty() const;

    int size() const;

    void display() const;

    void clear();
};

// Circular queue for worker shifts.
class CircularQueue
{
private:

    char* shifts;

    int capacity;
    int front;
    int rear;
    int count;

public:

    CircularQueue(int size = 5);

    ~CircularQueue();

    void enqueue(const char shift[]);

    void dequeue(char result[]);

    bool isEmpty() const;

    bool isFull() const;

    int size() const;

    void display() const;
};

#endif
