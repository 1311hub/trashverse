
#include "data.h"
#include <cstring>

// ==========================================
// HAVERSINE DISTANCE
// ==========================================

// Calculates distance between two geographical coordinates.
// Time Complexity: O(1)
double DataManager::haversine(
    double lat1,
    double lon1,
    double lat2,
    double lon2
)
{
    const double PI = 3.14159265358979323846;

    const double R = 6371.0;

    lat1 = lat1 * PI / 180.0;
    lon1 = lon1 * PI / 180.0;

    lat2 = lat2 * PI / 180.0;
    lon2 = lon2 * PI / 180.0;

    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    double a =
        sin(dLat / 2) * sin(dLat / 2) +
        cos(lat1) * cos(lat2) *
        sin(dLon / 2) * sin(dLon / 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c;
}

// ==========================================
// CHECK CSV FILES
// ==========================================

// Time Complexity: O(1)
bool DataManager::filesExist()
{
    ifstream binsFile("data/bins.csv");

    ifstream roadsFile("data/roads.csv");

    bool exists = binsFile.good() && roadsFile.good();

    binsFile.close();
    roadsFile.close();

    return exists;
}

// ==========================================
// GENERATE DATA
// ==========================================

// Fixed seed = 42.
// Time Complexity: O(V + E)
bool DataManager::generateData()
{
    srand(42);

    ofstream binsFile("data/bins.csv");

    ofstream roadsFile("data/roads.csv");

    if (!binsFile.is_open() || !roadsFile.is_open())
    {
        cout << "Unable to create CSV files.\n";

        return false;
    }

    binsFile << "bin_id,name,lat,lon,zone,fill_level\n";

    roadsFile << "bin_a,bin_b,distance_km\n";

    double latitude[31];

    double longitude[31];

    // Depot coordinates.
    latitude[0] = 28.6139;

    longitude[0] = 77.2090;

    // Generate 30 bins in a city-like grid.
    for (int i = 1; i <= 30; i++)
    {
        int row = (i - 1) / 6;

        int col = (i - 1) % 6;

        latitude[i] =
            28.6139 + row * 0.004 + (col % 2) * 0.0005;

        longitude[i] =
            77.2090 + col * 0.005 + row * 0.0007;

        int zone = (i - 1) / 10 + 1;

        int fill = 25 + rand() % 76;

        binsFile
            << i << ","
            << "Bin" << i << ","
            << latitude[i] << ","
            << longitude[i] << ","
            << zone << ","
            << fill << "\n";
    }

    // Create a connected ring:
    // Depot -> Bin1 -> Bin2 -> ... -> Bin30 -> Depot.

    for (int i = 0; i < 30; i++)
    {
        int u = i;

        int v = i + 1;

        double distance = haversine(
            latitude[u],
            longitude[u],
            latitude[v],
            longitude[v]
        );

        roadsFile << u << ","
                  << v << ","
                  << distance << "\n";
    }

    // Extra roads for alternative routes.
    for (int i = 1; i <= 24; i += 3)
    {
        int u = i;

        int v = i + 3;

        double distance = haversine(
            latitude[u],
            longitude[u],
            latitude[v],
            longitude[v]
        );

        roadsFile << u << ","
                  << v << ","
                  << distance << "\n";
    }

    // Additional connections.
    for (int i = 1; i <= 20; i += 5)
    {
        int u = i;

        int v = i + 10;

        double distance = haversine(
            latitude[u],
            longitude[u],
            latitude[v],
            longitude[v]
        );

        roadsFile << u << ","
                  << v << ","
                  << distance << "\n";
    }

    binsFile.close();

    roadsFile.close();

    cout << "\nDataset generated successfully!\n";

    cout << "30 bins created.\n";

    cout << "Connected road network created.\n";

    return true;
}

// ==========================================
// LOAD DATA
// ==========================================

// Time Complexity:
// O(B + E), excluding HashMap resizing and tree lookups.
bool DataManager::loadData(
    HashMap& bins,
    Graph& graph,
    ZoneTree& tree
)
{
    ifstream binsFile("data/bins.csv");

    ifstream roadsFile("data/roads.csv");

    if (!binsFile.is_open() || !roadsFile.is_open())
    {
        cout << "Unable to open CSV files.\n";

        return false;
    }

    string line;

    // Skip CSV header.
    getline(binsFile, line);

    // Load bin records.
    while (getline(binsFile, line))
    {
        stringstream ss(line);

        string id, name, lat, lon, zone, fill;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, lat, ',');
        getline(ss, lon, ',');
        getline(ss, zone, ',');
        getline(ss, fill, ',');

        BinRecord b;

        b.id = atoi(id.c_str());

        strncpy(b.name, name.c_str(), sizeof(b.name) - 1);

        b.name[sizeof(b.name) - 1] = '\0';

        b.lat = atof(lat.c_str());

        b.lon = atof(lon.c_str());

        b.zone_id = atoi(zone.c_str());

        b.fill_level = atoi(fill.c_str());

        b.worker_assigned = (b.id % 3) + 1;

        strcpy(b.last_collected_time, "Never");

        if (!bins.insert(b))
        {
            cout << "Duplicate bin ID: " << b.id << endl;
        }

        // Create zones.
        char zoneName[40];

        sprintf(zoneName, "Zone %d", b.zone_id);

        tree.addZone(b.zone_id, zoneName);

        // Create two subzones per zone.
        int subzone = 100 + b.zone_id * 10 + ((b.id - 1) % 10) / 5;

        char subzoneName[40];

        sprintf(
            subzoneName,
            "Subzone %d",
            subzone
        );

        tree.addSubzone(
            b.zone_id,
            subzone,
            subzoneName
        );

        // Add bin to tree.
        tree.addBin(
            subzone,
            b.id,
            b.name,
            b.fill_level
        );
    }

    // Load roads.
    getline(roadsFile, line);

    while (getline(roadsFile, line))
    {
        stringstream ss(line);

        string a, b, distance;

        getline(ss, a, ',');
        getline(ss, b, ',');
        getline(ss, distance, ',');

        int u = atoi(a.c_str());

        int v = atoi(b.c_str());

        double weight = atof(distance.c_str());

        graph.addEdge(u, v, weight);
    }

    binsFile.close();

    roadsFile.close();

    cout << "\nCSV data loaded successfully!\n";

    cout << "Total bins loaded: " << bins.size() << endl;

    cout << "Graph nodes: " << graph.getVertices() << endl;

    return bins.size() == 30;
}

// ==========================================
// SIMULATE FILL GROWTH
// ==========================================

// Time Complexity: O(B * T)
// B = number of bins, T = number of ticks.
void DataManager::simulateGrowth(
    HashMap& bins,
    int ticks
)
{
    for (int t = 0; t < ticks; t++)
    {
        for (int id = 1; id <= 30; id++)
        {
            BinRecord* b = bins.get(id);

            if (b != NULL)
            {
                // Random increase between 1 and 5.
                int increase = 1 + rand() % 5;

                b->fill_level += increase;

                if (b->fill_level > 100)
                    b->fill_level = 100;
            }
        }
    }

    cout << "\nTime simulation completed.\n";

    cout << "Ticks simulated: " << ticks << endl;
}
