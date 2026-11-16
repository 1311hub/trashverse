
#include "display.h"
#include <iomanip>

// ANSI color codes.
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define RESET   "\033[0m"
#define CYAN    "\033[36m"

// ==========================================
// SHOW BIN TABLE
// ==========================================

// Time Complexity: O(B)
void Display::showBins(HashMap& bins)
{
    cout << "\n========== BIN MONITORING TABLE ==========\n";

    cout << left
         << setw(10) << "ID"
         << setw(12) << "Name"
         << setw(12) << "Fill"
         << setw(10) << "Zone"
         << setw(15) << "Worker"
         << "Last Collected\n";

    cout << "----------------------------------------------------------\n";

    for (int i = 1; i <= 30; i++)
    {
        BinRecord* b = bins.get(i);

        if (b == NULL)
            continue;

        const char* color;

        if (b->fill_level >= 80)
            color = RED;
        else if (b->fill_level >= 50)
            color = YELLOW;
        else
            color = GREEN;

        cout << left
             << setw(10) << b->id
             << setw(12) << b->name;

        cout << color
             << setw(12) << (to_string(b->fill_level) + "%")
             << RESET;

        cout << setw(10) << b->zone_id
             << setw(15) << b->worker_assigned
             << b->last_collected_time
             << endl;
    }
}

// ==========================================
// ASCII MAP
// ==========================================

// Time Complexity: O(B)
void Display::showMap(HashMap& bins)
{
    cout << "\n========== CITY BIN MAP ==========\n";

    cout << GREEN << "G = Below 50%" << RESET << endl;

    cout << YELLOW << "Y = 50-79%" << RESET << endl;

    cout << RED << "R = 80-100%" << RESET << endl;

    cout << "\n";

    for (int i = 1; i <= 30; i++)
    {
        BinRecord* b = bins.get(i);

        if (b == NULL)
            continue;

        const char* color;

        char status;

        if (b->fill_level >= 80)
        {
            color = RED;
            status = 'R';
        }
        else if (b->fill_level >= 50)
        {
            color = YELLOW;
            status = 'Y';
        }
        else
        {
            color = GREEN;
            status = 'G';
        }

        cout << color
             << "[B" << setw(2) << setfill('0') << i
             << ":" << status << "] "
             << RESET;

        if (i % 6 == 0)
            cout << endl;
    }

    cout << setfill(' ') << endl;
}

// ==========================================
// SHOW ROUTE
// ==========================================

// Time Complexity: O(V + N)
void Display::showRoute(RouteResult& route)
{
    cout << "\n========== COLLECTION ROUTE ==========\n";

    if (route.visitCount <= 1)
    {
        cout << "No urgent bins to collect.\n";
        return;
    }

    cout << "\nOptimal visiting order (nearest-neighbour heuristic):\n";

    for (int i = 0; i < route.visitCount; i++)
    {
        if (i == 0)
            cout << "Depot";
        else
            cout << " -> B" << route.visitOrder[i];
    }

    cout << "\n\nDistance per leg:\n";

    for (int i = 1; i < route.visitCount; i++)
    {
        int from = route.visitOrder[i - 1];

        int to = route.visitOrder[i];

        cout << "Node " << from
             << " -> Node " << to
             << " : "
             << fixed << setprecision(2)
             << route.legDistances[i - 1]
             << " km\n";
    }

    cout << "\nFull road path:\n";

    for (int i = 0; i < route.pathCount; i++)
    {
        if (route.fullPath[i] == 0)
            cout << "Depot";
        else
            cout << "B" << route.fullPath[i];

        if (i != route.pathCount - 1)
            cout << " -> ";
    }

    cout << "\n\nTotal distance: "
         << fixed << setprecision(2)
         << route.totalDistance
         << " km\n";
}

// ==========================================
// ZONE STATISTICS
// ==========================================

void Display::showZoneStats(ZoneTree& tree)
{
    tree.display();

    tree.displayZoneStats();

    tree.displayWorstZone();
}

// ==========================================
// PAYMENT REPORT
// ==========================================

void Display::showPaymentReport(WorkerLedger& ledger)
{
    ledger.displayReport();
}

// ==========================================
// MAIN MENU
// ==========================================

void Display::showMenu()
{
    cout << "\n";
    cout << CYAN;

    cout << "==========================================\n";

    cout << " SMART WASTE COLLECTION SYSTEM\n";

    cout << "==========================================\n";

    cout << RESET;

    cout << "[1] Run full collection cycle\n";

    cout << "[2] Simulate time passing\n";

    cout << "[3] Show bins\n";

    cout << "[4] Show zone statistics\n";

    cout << "[5] Show payment report\n";

    cout << "[6] Regenerate data\n";

    cout << "[0] Exit\n";

    cout << "\nEnter choice: ";
}
