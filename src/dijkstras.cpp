#include "dijkstras.h"

vector<int> dijkstra_shortest_path(const Graph& graph, int source, vector<int>& previous) {
    int numVertices = graph.numVertices;
    vector<int> distances(numVertices, INF);
    previous.assign(numVertices, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> minHeap;

    distances[source] = 0;
    minHeap.emplace(0, source);

    while (!minHeap.empty()) {
        auto [currDist, u] = minHeap.top();
        minHeap.pop();

        if (currDist > distances[u]) continue;

        for (const Edge& edge : graph[u]) {
            int v = edge.dst, weight = edge.weight;
            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;
                minHeap.emplace(distances[v], v);
            }
        }
    }

    return distances;
}

vector<int> extract_shortest_path(const vector<int>& previous, int destination) {
    vector<int> path;
    for (int i = destination; i != -1; i = previous[i]) {
        path.push_back(i);
    }
    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<int>& path, int total) {
    if (path.empty()) {
        cout << "No path found" << endl;
        return;
    }

    cout << "Shortest path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i < path.size() - 1) cout << " ";
    }
    cout << "\nTotal distance: " << total << endl;
}
