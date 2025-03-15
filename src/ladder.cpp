#ifndef WORD_LADDER_H
#define WORD_LADDER_H

#include "dijkstra.h"
#include "ladder.h"

void error(const string& word1, const string& word2, const string& msg) {
    cerr << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
}

bool is_adjacent(const string& word1, const string& word2) {
    if (word1.length() != word2.length()) return false;
    int count = 0;
    for (size_t i = 0; i < word1.length(); ++i) {
        if (word1[i] != word2[i]) count++;
        if (count > 1) return false;
    }
    return count == 1;
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file) {
        cerr << "Cannot open file: " << file_name << endl;
        return;
    }
    string word;
    while (file >> word) {
        word_list.insert(word);
    }
    file.close();
}

void WordLadderGraph::build_graph(const set<string>& word_list) {
    for (const string& word : word_list) {
        for (const string& potential : word_list) {
            if (is_adjacent(word, potential)) {
                adjacency_list[word].push_back(potential);
            }
        }
    }
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (word_list.find(begin_word) == word_list.end() || word_list.find(end_word) == word_list.end()) {
        return {};
    }
    WordLadderGraph graph;
    graph.build_graph(word_list);
    
    map<string, string> previous;
    map<string, int> distances;
    for (const string& word : word_list) distances[word] = INT_MAX;
    distances[begin_word] = 0;
    
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    pq.emplace(0, begin_word);
    
    while (!pq.empty()) {
        auto [dist, word] = pq.top(); pq.pop();
        if (word == end_word) break;
        for (const string& neighbor : graph.adjacency_list[word]) {
            if (dist + 1 < distances[neighbor]) {
                distances[neighbor] = dist + 1;
                previous[neighbor] = word;
                pq.emplace(distances[neighbor], neighbor);
            }
        }
    }
    
    if (distances[end_word] == INT_MAX) return {};
    
    vector<string> path;
    for (string at = end_word; !at.empty(); at = previous[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    for (size_t i = 0; i < ladder.size(); ++i) {
        cout << ladder[i];
        if (i < ladder.size() - 1) cout << " -> ";
    }
    cout << endl;
}

#endif