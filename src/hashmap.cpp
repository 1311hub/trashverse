
#include "hashmap.h"

// Constructor
HashMap::HashMap(int size)
{
    capacity = size;
    count = 0;

    table = new HashNode*[capacity];

    for (int i = 0; i < capacity; i++)
        table[i] = NULL;
}

// Hash function
// Time Complexity: O(1)
int HashMap::hashFunction(int key) const
{
    if (key < 0)
        key = -key;

    return key % capacity;
}

// Insert
// Average Time Complexity: O(1)
// Worst Case: O(n), when all keys collide.
bool HashMap::insert(BinRecord b)
{
    int index = hashFunction(b.id);

    HashNode* temp = table[index];

    // Do not allow duplicate IDs.
    while (temp != NULL)
    {
        if (temp->data.id == b.id)
            return false;

        temp = temp->next;
    }

    HashNode* newNode = new HashNode(b);

    newNode->next = table[index];
    table[index] = newNode;

    count++;

    // Resize if load factor exceeds 0.75.
    if ((double)count / capacity > 0.75)
        resize();

    return true;
}

// Get
// Average Time Complexity: O(1)
// Worst Case: O(n)
BinRecord* HashMap::get(int id)
{
    int index = hashFunction(id);

    HashNode* temp = table[index];

    while (temp != NULL)
    {
        if (temp->data.id == id)
            return &temp->data;

        temp = temp->next;
    }

    return NULL;
}

// Contains
// Average Time Complexity: O(1)
// Worst Case: O(n)
bool HashMap::contains(int id)
{
    return get(id) != NULL;
}

// Update
// Average Time Complexity: O(1)
// Worst Case: O(n)
bool HashMap::update(BinRecord b)
{
    BinRecord* existing = get(b.id);

    if (existing == NULL)
        return false;

    *existing = b;

    return true;
}

// Remove
// Average Time Complexity: O(1)
// Worst Case: O(n)
bool HashMap::remove(int id)
{
    int index = hashFunction(id);

    HashNode* temp = table[index];
    HashNode* previous = NULL;

    while (temp != NULL)
    {
        if (temp->data.id == id)
        {
            if (previous == NULL)
                table[index] = temp->next;
            else
                previous->next = temp->next;

            delete temp;
            count--;

            return true;
        }

        previous = temp;
        temp = temp->next;
    }

    return false;
}

// Resize
// Time Complexity: O(n) for rehashing all existing elements.
void HashMap::resize()
{
    int oldCapacity = capacity;
    HashNode** oldTable = table;

    capacity = capacity * 2 + 1;

    table = new HashNode*[capacity];

    for (int i = 0; i < capacity; i++)
        table[i] = NULL;

    for (int i = 0; i < oldCapacity; i++)
    {
        HashNode* temp = oldTable[i];

        while (temp != NULL)
        {
            HashNode* nextNode = temp->next;

            int index = hashFunction(temp->data.id);

            temp->next = table[index];
            table[index] = temp;

            temp = nextNode;
        }
    }

    delete[] oldTable;
}

// Size
// Time Complexity: O(1)
int HashMap::size() const
{
    return count;
}

// Is Empty
// Time Complexity: O(1)
bool HashMap::isEmpty() const
{
    return count == 0;
}

// Display
// Time Complexity: O(capacity + n)
void HashMap::display() const
{
    cout << "\n========== HASHMAP RECORDS ==========\n";

    for (int i = 0; i < capacity; i++)
    {
        HashNode* temp = table[i];

        while (temp != NULL)
        {
            cout << "ID: " << temp->data.id
                 << "  Name: " << temp->data.name
                 << "  Fill: " << temp->data.fill_level
                 << "%  Zone: " << temp->data.zone_id
                 << endl;

            temp = temp->next;
        }
    }
}

// Clear
// Time Complexity: O(capacity + n)
void HashMap::clear()
{
    for (int i = 0; i < capacity; i++)
    {
        HashNode* temp = table[i];

        while (temp != NULL)
        {
            HashNode* nextNode = temp->next;

            delete temp;

            temp = nextNode;
        }

        table[i] = NULL;
    }

    count = 0;
}

// Destructor
HashMap::~HashMap()
{
    clear();

    delete[] table;
}
