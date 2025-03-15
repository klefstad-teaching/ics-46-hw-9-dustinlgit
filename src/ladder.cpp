#include "ladder.h"
#include "dijkstras.h"

// Check if two words differ by exactly one letter
bool is_adjacent(const string& word1, const string& word2) {
    int count = 0;
    for (size_t i = 0; i < word1.size(); i++) {
        if (word1[i] != word2[i]) count++;
        if (count > 1) return false;
    }
    return count == 1;
}

// Convert words into a graph representation for Dijkstra
void build_graph(const set<string>& word_list, map<string, int>& word_index, Graph& G) {
    int index = 0;
    for (const string& word : word_list) {
        word_index[word] = index++;
    }
    G.numVertices = word_list.size();
    G.resize(G.numVertices);

    for (const string& w1 : word_list) {
        for (const string& w2 : word_list) {
            if (is_adjacent(w1, w2)) {
                G[word_index[w1]].push_back(Edge(word_index[w1], word_index[w2], 1));
            }
        }
    }
}

// Generate the shortest word ladder using Dijkstra
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (word_list.find(begin_word) == word_list.end() || word_list.find(end_word) == word_list.end()) {
        return {};
    }

    map<string, int> word_index;
    Graph G;
    build_graph(word_list, word_index, G);

    vector<int> previous(G.numVertices, -1);
    vector<int> distances = dijkstra_shortest_path(G, word_index[begin_word], previous);

    // Extract the shortest path
    vector<int> path_indices = extract_shortest_path(distances, previous, word_index[end_word]);
    if (path_indices.empty()) return {};

    // Convert indices back to words
    vector<string> path;
    map<int, string> index_word;
    for (const auto& [word, idx] : word_index) {
        index_word[idx] = word;
    }
    for (int idx : path_indices) {
        path.push_back(index_word[idx]);
    }

    return path;
}

// Print the word ladder
void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No transformation sequence found." << endl;
    } else {
        for (const string& word : ladder) {
            cout << word << " ";
        }
        cout << endl;
    }
}
