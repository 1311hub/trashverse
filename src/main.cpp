
#include <iostream>
#include <cstring>
#include "hashmap.h"
#include "heap.h"
#include "queue.h"
#include "graph.h"
#include "tree.h"
#include "ledger.h"
#include "data.h"
#include "display.h"

using namespace std;

void runCollectionCycle(
    HashMap& bins,
    MaxHeap& heap,
    LinkedQueue& queue,
    Graph& graph,
    ZoneTree& tree,
    WorkerLedger& ledger
)
{
    heap = MaxHeap(30);

    for (int i = 1; i <= 30; i++)
    {
        BinRecord* b = bins.get(i);

        if (b != NULL && b->fill_level >= 80)
            heap.insert(b->id, b->fill_level);
    }

    if (heap.isEmpty())
    {
        cout << "\nNo urgent bins require collection.\n";
        return;
    }

    while (!heap.isEmpty())
    {
        HeapNode node = heap.extractMax();
        queue.enqueue(node.bin_id);
    }

    int urgentBins[30];
    int count = 0;

    while (!queue.isEmpty())
    {
        urgentBins[count++] = queue.dequeue();
    }

    RouteResult route = graph.planRoute(urgentBins, count, 0);

    Display::showRoute(route);

    for (int i = 0; i < count; i++)
    {
        int id = urgentBins[i];
        BinRecord* b = bins.get(id);

        if (b == NULL)
            continue;

        int worker = b->worker_assigned;
        double distance = 0;

        for (int j = 1; j < route.visitCount; j++)
        {
            if (route.visitOrder[j] == id)
            {
                distance = route.legDistances[j - 1];
                break;
            }
        }

        ledger.recordCollection(worker, 25, distance);

        b->fill_level = 0;
        strcpy(b->last_collected_time, "Collected");

        tree.updateBin(id, 0);
    }

    cout << "\nCollection cycle completed.\n";
    cout << "Bins collected: " << count << endl;
}

int main()
{
    HashMap bins(53);
    MaxHeap heap(30);
    LinkedQueue queue;
    Graph graph(31);
    ZoneTree tree;
    WorkerLedger ledger(30, 200);

    ledger.addWorker(1, "Worker1", 500);
    ledger.addWorker(2, "Worker2", 500);
    ledger.addWorker(3, "Worker3", 500);

    if (!DataManager::filesExist())
    {
        if (!DataManager::generateData())
            return 1;
    }

    if (!DataManager::loadData(bins, graph, tree))
    {
        cout << "Unable to load project data.\n";
        return 1;
    }

    int choice;

    do
    {
        Display::showMenu();

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
                runCollectionCycle(
                    bins, heap, queue, graph, tree, ledger
                );
                break;

            case 2:
            {
                int ticks;
                cout << "Enter number of time ticks: ";
                cin >> ticks;

                if (ticks < 0)
                {
                    cout << "Ticks cannot be negative.\n";
                    break;
                }

                DataManager::simulateGrowth(bins, ticks);

                for (int i = 1; i <= 30; i++)
                {
                    BinRecord* b = bins.get(i);

                    if (b != NULL)
                        tree.updateBin(i, b->fill_level);
                }

                break;
            }

            case 3:
                Display::showBins(bins);
                Display::showMap(bins);
                break;

            case 4:
                Display::showZoneStats(tree);
                break;

            case 5:
                Display::showPaymentReport(ledger);
                break;

            case 6:
                if (DataManager::generateData())
                    cout << "Restart the program to load regenerated data.\n";
                break;

            case 0:
                cout << "Exiting Smart Waste Collection System.\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
