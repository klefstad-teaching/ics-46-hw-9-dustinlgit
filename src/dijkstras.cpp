#include "dijkstras.h"

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous){
    int n = G.numVertices;
    vector<int> distances(n, INF);
    previous.assign(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    
    distances[source] = 0;
    pq.emplace(0, source);
    
    while (!pq.empty()) {
        auto [dist, u] = pq.top();
        pq.pop();
        
        if (dist > distances[u]) continue;
        
        for (const Edge& e : G[u]) {
            int v = e.dst, weight = e.weight;
            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;
                pq.emplace(distances[v], v);
            }
        }
    }
    
    return distances;
}
vector<int> extract_shortest_path(const vector<int>& /*distances*/, const vector<int>& previous, int destination){
    vector<int> path;
    for (int i = destination; i != -1; at = previous[i]) {
        path.push_back(i);
    }
    reverse(path.begin(), path.end());
    return path;
}
void print_path(const vector<int>& v, int total){
    if (v.empty() || v[0] == total) {
        cout << "No path found" << endl;
        return;
    }
    
    cout << "Shortest path: ";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i];
        if (i < v.size() - 1) cout << " -> ";
    }
    cout << "\nTotal distance: " << total << endl;
}