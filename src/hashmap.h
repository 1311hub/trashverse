
#ifndef HASHMAP_H
#define HASHMAP_H

#include <iostream>
#include <cstring>
using namespace std;

// Structure to store complete information about one garbage bin.
struct BinRecord
{
    int id;
    char name[30];

    double lat;
    double lon;

    int fill_level;
    int zone_id;

    char last_collected_time[40];
    int worker_assigned;

    BinRecord()
    {
        id = 0;
        strcpy(name, "Unknown");

        lat = 0;
        lon = 0;

        fill_level = 0;
        zone_id = 0;

        strcpy(last_collected_time, "Never");
        worker_assigned = 0;
    }
};

// Node used in separate chaining.
struct HashNode
{
    BinRecord data;
    HashNode* next;

    HashNode(BinRecord b)
    {
        data = b;
        next = NULL;
    }
};

class HashMap
{
private:

    HashNode** table;

    int capacity;
    int count;

    // Hash function: maps bin ID to a table index.
    int hashFunction(int key) const;

    // Resize table when load factor becomes too high.
    void resize();

public:

    // Constructor
    HashMap(int size = 53);

    // Destructor
    ~HashMap();

    // Insert a new bin.
    bool insert(BinRecord b);

    // Search for a bin.
    BinRecord* get(int id);

    // Update an existing bin.
    bool update(BinRecord b);

    // Delete a bin.
    bool remove(int id);

    // Check whether bin exists.
    bool contains(int id);

    // Number of stored bins.
    int size() const;

    // Check whether map is empty.
    bool isEmpty() const;

    // Display all records.
    void display() const;

    // Remove all records.
    void clear();
};

#endif
