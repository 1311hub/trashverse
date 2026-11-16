
#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cstring>
using namespace std;

struct TreeNode
{
    int id;

    char name[40];

    int fill_level;

    int type;
    // 0 = City
    // 1 = Zone
    // 2 = Subzone
    // 3 = Bin

    TreeNode* firstChild;
    TreeNode* nextSibling;

    TreeNode(int i, const char n[], int t, int fill = 0)
    {
        id = i;

        strcpy(name, n);

        type = t;

        fill_level = fill;

        firstChild = NULL;
        nextSibling = NULL;
    }
};

struct ZoneStats
{
    int totalBins;
    int overflowBins;
    double totalFill;
    double averageFill;

    ZoneStats()
    {
        totalBins = 0;
        overflowBins = 0;
        totalFill = 0;
        averageFill = 0;
    }
};

class ZoneTree
{
private:

    TreeNode* root;

    void calculateStats(TreeNode* node, ZoneStats& stats);

    void printTree(TreeNode* node, int depth);

    void deleteTree(TreeNode* node);

    TreeNode* findNode(TreeNode* node, int id, int type);

public:

    ZoneTree();

    ~ZoneTree();

    void addZone(int zone_id, const char name[]);

    void addSubzone(
        int zone_id,
        int subzone_id,
        const char name[]
    );

    void addBin(
        int subzone_id,
        int bin_id,
        const char name[],
        int fill
    );

    bool updateBin(int bin_id, int fill);

    void display();

    void displayZoneStats();

    void displayWorstZone();
};

#endif
