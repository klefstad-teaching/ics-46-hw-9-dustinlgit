#include "ladder.h"
#include "dijkstras.h"


sing namespace std;

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    if (abs((int)str1.size() - (int)str2.size()) > d) return false;
    int differences = 0;
    for (size_t i = 0; i < min(str1.size(), str2.size()); ++i) {
        if (str1[i] != str2[i]) {
            differences++;
            if (differences > d) return false;
        }
    }
    return differences <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file) {
        cerr << "Error: Could not open " << file_name << endl;
        return;
    }
    string word;
    while (file >> word) {
        word_list.insert(word);
    }
    file.close();
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    Graph G;
    map<string, int> word_to_index;
    vector<string> index_to_word;
    
    int index = 0;
    for (const auto& word : word_list) {
        word_to_index[word] = index++;
        index_to_word.push_back(word);
    }
    
    G.numVertices = word_list.size();
    G.resize(G.numVertices);
    
    for (const auto& word1 : word_list) {
        for (const auto& word2 : word_list) {
            if (is_adjacent(word1, word2)) {
                int u = word_to_index[word1];
                int v = word_to_index[word2];
                G[u].push_back(Edge(u, v, 1));
            }
        }
    }
    
    if (word_to_index.find(begin_word) == word_to_index.end() ||
        word_to_index.find(end_word) == word_to_index.end()) {
        return {};
    }
    
    int source = word_to_index[begin_word];
    int destination = word_to_index[end_word];
    vector<int> previous(G.numVertices, -1);
    vector<int> distances = dijkstra_shortest_path(G, source, previous);
    vector<int> path_indices = extract_shortest_path(distances, previous, destination);
    
    vector<string> path;
    for (int idx : path_indices) {
        path.push_back(index_to_word[idx]);
    }
    return path;
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    for (const string& word : ladder) {
        cout << word << " -> ";
    }
    cout << "END" << endl;
}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    string start_word = "hit", end_word = "cog";
    vector<string> ladder = generate_word_ladder(start_word, end_word, word_list);
    print_word_ladder(ladder);
}