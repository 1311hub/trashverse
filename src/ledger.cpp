
#include "ledger.h"

// Constructor
// Time Complexity: O(1)
WorkerLedger::WorkerLedger(
    double threshold,
    double bonus
)
{
    workerCount = 0;

    responseThreshold = threshold;

    bonusAmount = bonus;
}

// Find Worker
// Time Complexity: O(n)
int WorkerLedger::findWorker(int id) const
{
    for (int i = 0; i < workerCount; i++)
    {
        if (workers[i].worker_id == id)
            return i;
    }

    return -1;
}

// Add Worker
// Time Complexity: O(n) for duplicate check.
bool WorkerLedger::addWorker(
    int id,
    const char name[],
    double baseRate
)
{
    if (workerCount >= MAX_WORKERS)
        return false;

    if (findWorker(id) != -1)
        return false;

    workers[workerCount].worker_id = id;

    strncpy(
        workers[workerCount].name,
        name,
        sizeof(workers[workerCount].name) - 1
    );

    workers[workerCount].name[
        sizeof(workers[workerCount].name) - 1
    ] = '\0';

    workers[workerCount].base_rate = baseRate;

    workers[workerCount].bonus = 0;

    workerCount++;

    return true;
}

// Record Collection
// Time Complexity: O(n) for worker lookup.
bool WorkerLedger::recordCollection(
    int worker_id,
    double responseTime,
    double distance
)
{
    int index = findWorker(worker_id);

    if (index == -1 ||
        responseTime < 0 ||
        distance < 0)
    {
        return false;
    }

    workers[index].collections_completed++;

    workers[index].total_response_time += responseTime;

    workers[index].total_distance += distance;

    return true;
}

// Calculate Payment
// Time Complexity: O(n) for worker lookup.
double WorkerLedger::calculatePay(int worker_id) const
{
    int index = findWorker(worker_id);

    if (index == -1)
        return 0;

    double averageResponse = 0;

    if (workers[index].collections_completed > 0)
    {
        averageResponse =
            workers[index].total_response_time /
            workers[index].collections_completed;
    }

    double bonus = 0;

    if (workers[index].collections_completed > 0 &&
        averageResponse < responseThreshold)
    {
        bonus = bonusAmount;
    }

    return workers[index].base_rate + bonus;
}

// Payment Report
// Time Complexity: O(n)
void WorkerLedger::displayReport() const
{
    cout << "\n========== WORKER PAYMENT REPORT ==========\n";

    cout << "Response threshold: "
         << responseThreshold << " minutes\n";

    cout << "Performance bonus: Rs. "
         << bonusAmount << "\n\n";

    cout << "ID\tName\tCollections\tAvg Time\tDistance\tPay\n";

    for (int i = 0; i < workerCount; i++)
    {
        double averageResponse = 0;

        if (workers[i].collections_completed > 0)
        {
            averageResponse =
                workers[i].total_response_time /
                workers[i].collections_completed;
        }

        cout << workers[i].worker_id << "\t";

        cout << workers[i].name << "\t";

        cout << workers[i].collections_completed << "\t\t";

        cout << averageResponse << " min\t";

        cout << workers[i].total_distance << " km\t";

        cout << "Rs. " << calculatePay(
            workers[i].worker_id
        );

        cout << endl;
    }
}

// Worker Count
// Time Complexity: O(1)
int WorkerLedger::getWorkerCount() const
{
    return workerCount;
}
