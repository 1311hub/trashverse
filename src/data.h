
#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include "hashmap.h"
#include "graph.h"
#include "tree.h"

using namespace std;

class DataManager
{
private:

    static double haversine(
        double lat1,
        double lon1,
        double lat2,
        double lon2
    );

public:

    // Generate bins.csv and roads.csv.
    static bool generateData();

    // Check whether CSV files exist.
    static bool filesExist();

    // Load data into all custom structures.
    static bool loadData(
        HashMap& bins,
        Graph& graph,
        ZoneTree& tree
    );

    // Simulate fill-level growth.
    static void simulateGrowth(
        HashMap& bins,
        int ticks
    );

};

#endif
