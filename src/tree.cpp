
#include "tree.h"

// Constructor
// Time Complexity: O(1)
ZoneTree::ZoneTree()
{
    root = new TreeNode(0, "City", 0);
}

// Find Node
// Time Complexity: O(n)
TreeNode* ZoneTree::findNode(
    TreeNode* node,
    int id,
    int type
)
{
    if (node == NULL)
        return NULL;

    if (node->id == id && node->type == type)
        return node;

    TreeNode* result = findNode(
        node->firstChild,
        id,
        type
    );

    if (result != NULL)
        return result;

    return findNode(
        node->nextSibling,
        id,
        type
    );
}

// Add Zone
// Time Complexity: O(number of existing nodes)
void ZoneTree::addZone(int zone_id, const char name[])
{
    if (findNode(root, zone_id, 1) != NULL)
        return;

    TreeNode* newNode = new TreeNode(
        zone_id,
        name,
        1
    );

    newNode->nextSibling = root->firstChild;

    root->firstChild = newNode;
}

// Add Subzone
// Time Complexity: O(n)
void ZoneTree::addSubzone(
    int zone_id,
    int subzone_id,
    const char name[]
)
{
    TreeNode* zone = findNode(root, zone_id, 1);

    if (zone == NULL ||
        findNode(root, subzone_id, 2) != NULL)
    {
        return;
    }

    TreeNode* newNode = new TreeNode(
        subzone_id,
        name,
        2
    );

    newNode->nextSibling = zone->firstChild;

    zone->firstChild = newNode;
}

// Add Bin
// Time Complexity: O(n)
void ZoneTree::addBin(
    int subzone_id,
    int bin_id,
    const char name[],
    int fill
)
{
    TreeNode* subzone = findNode(root, subzone_id, 2);

    if (subzone == NULL ||
        findNode(root, bin_id, 3) != NULL)
    {
        return;
    }

    TreeNode* newNode = new TreeNode(
        bin_id,
        name,
        3,
        fill
    );

    newNode->nextSibling = subzone->firstChild;

    subzone->firstChild = newNode;
}

// Update Bin
// Time Complexity: O(n)
bool ZoneTree::updateBin(int bin_id, int fill)
{
    TreeNode* bin = findNode(root, bin_id, 3);

    if (bin == NULL)
        return false;

    bin->fill_level = fill;

    return true;
}

// Recursive Statistics
// Time Complexity: O(n)
void ZoneTree::calculateStats(
    TreeNode* node,
    ZoneStats& stats
)
{
    if (node == NULL)
        return;

    if (node->type == 3)
    {
        stats.totalBins++;

        stats.totalFill += node->fill_level;

        if (node->fill_level >= 100)
            stats.overflowBins++;
    }

    calculateStats(node->firstChild, stats);

    calculateStats(node->nextSibling, stats);
}

// Print Tree
// Time Complexity: O(n)
void ZoneTree::printTree(TreeNode* node, int depth)
{
    if (node == NULL)
        return;

    for (int i = 0; i < depth; i++)
        cout << "    ";

    if (node->type == 0)
        cout << "[CITY] ";
    else if (node->type == 1)
        cout << "[ZONE] ";
    else if (node->type == 2)
        cout << "[SUBZONE] ";
    else
        cout << "[BIN] ";

    cout << node->name;

    if (node->type == 3)
        cout << " (" << node->fill_level << "%)";

    cout << endl;

    printTree(node->firstChild, depth + 1);

    printTree(node->nextSibling, depth);
}

// Display Tree
void ZoneTree::display()
{
    cout << "\n========== CITY ZONE TREE ==========\n";

    printTree(root, 0);
}

// Display Zone Statistics
// Time Complexity: O(n) overall when visiting each zone's bins.
void ZoneTree::displayZoneStats()
{
    cout << "\n========== ZONE STATISTICS ==========\n";

    TreeNode* zone = root->firstChild;

    while (zone != NULL)
    {
        if (zone->type == 1)
        {
            ZoneStats stats;

            calculateStats(zone, stats);

            if (stats.totalBins > 0)
            {
                stats.averageFill =
                    stats.totalFill / stats.totalBins;
            }

            cout << "\nZone: " << zone->name << endl;

            cout << "Total bins: "
                 << stats.totalBins << endl;

            cout << "Average fill: "
                 << stats.averageFill << "%" << endl;

            cout << "Overflow bins: "
                 << stats.overflowBins << endl;
        }

        zone = zone->nextSibling;
    }
}

// Display Worst Zone
// Time Complexity: O(n)
void ZoneTree::displayWorstZone()
{
    TreeNode* zone = root->firstChild;

    TreeNode* worstZone = NULL;

    double maximumAverage = -1;

    while (zone != NULL)
    {
        if (zone->type == 1)
        {
            ZoneStats stats;

            calculateStats(zone, stats);

            if (stats.totalBins > 0)
            {
                double average =
                    stats.totalFill / stats.totalBins;

                if (average > maximumAverage)
                {
                    maximumAverage = average;

                    worstZone = zone;
                }
            }
        }

        zone = zone->nextSibling;
    }

    if (worstZone != NULL)
    {
        cout << "\nWorst Zone: "
             << worstZone->name << endl;

        cout << "Average fill: "
             << maximumAverage << "%" << endl;
    }
    else
    {
        cout << "\nNo zone data available.\n";
    }
}

// Delete Tree
// Time Complexity: O(n)
void ZoneTree::deleteTree(TreeNode* node)
{
    if (node == NULL)
        return;

    deleteTree(node->firstChild);

    deleteTree(node->nextSibling);

    delete node;
}

// Destructor
ZoneTree::~ZoneTree()
{
    deleteTree(root);
}
