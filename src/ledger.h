
#ifndef LEDGER_H
#define LEDGER_H

#include <iostream>
#include <cstring>
using namespace std;

#define MAX_WORKERS 20

struct Worker
{
    int worker_id;

    char name[40];

    int collections_completed;

    double total_response_time;

    double total_distance;

    double base_rate;

    double bonus;

    Worker()
    {
        worker_id = 0;

        strcpy(name, "Unknown");

        collections_completed = 0;

        total_response_time = 0;

        total_distance = 0;

        base_rate = 500;

        bonus = 0;
    }
};

class WorkerLedger
{
private:

    Worker workers[MAX_WORKERS];

    int workerCount;

    double responseThreshold;

    double bonusAmount;

    int findWorker(int id) const;

public:

    WorkerLedger(
        double threshold = 30,
        double bonus = 200
    );

    bool addWorker(
        int id,
        const char name[],
        double baseRate
    );

    bool recordCollection(
        int worker_id,
        double responseTime,
        double distance
    );

    double calculatePay(int worker_id) const;

    void displayReport() const;

    int getWorkerCount() const;
};

#endif
