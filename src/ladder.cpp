#ifndef WORD_LADDER_H
#define WORD_LADDER_H

#include "dijkstras.h"
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

struct WordLadderGraph {
    map<string, vector<Edge>> adjacency_list;
    map<string, int> word_to_index;
    vector<string> index_to_word;

    void build_graph(const set<string>& word_list) {
        int index = 0;
        for (const string& word : word_list) {
            word_to_index[word] = index;
            index_to_word.push_back(word);
            index++;
        }
        
        for (const string& word : word_list) {
            int u = word_to_index[word];
            for (const string& potential : word_list) {
                if (is_adjacent(word, potential)) {
                    int v = word_to_index[potential];
                    adjacency_list[word].push_back(Edge(u, v, 1));
                }
            }
        }
    }
};

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (word_list.find(begin_word) == word_list.end() || word_list.find(end_word) == word_list.end()) {
        return {};
    }
    WordLadderGraph graph;
    graph.build_graph(word_list);

    int source = graph.word_to_index[begin_word];
    int destination = graph.word_to_index[end_word];
    vector<int> previous;
    
    vector<int> distances = dijkstra_shortest_path(graph.adjacency_list, source, previous);
    
    if (distances[destination] == INF) return {};
    
    vector<int> path_indices = extract_shortest_path(distances, previous, destination);
    vector<string> path;
    for (int index : path_indices) {
        path.push_back(graph.index_to_word[index]);
    }
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