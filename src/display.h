
#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include "hashmap.h"
#include "graph.h"
#include "tree.h"
#include "ledger.h"

using namespace std;

class Display
{
public:

    static void showBins(HashMap& bins);

    static void showMap(HashMap& bins);

    static void showRoute(RouteResult& route);

    static void showZoneStats(ZoneTree& tree);

    static void showPaymentReport(WorkerLedger& ledger);

    static void showMenu();
};

#endif
