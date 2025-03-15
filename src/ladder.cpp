#include "ladder.h"
#include "dijkstras.h"

void error(string word1, string word2, string msg) {
    cout << "Error: " << msg << " (" << word1 << " " << word2 << ")" << endl;
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    if (str1.length() != str2.length()) return false;
    int count = 0;
    for (size_t i = 0; i < str1.length(); ++i) {
        if (str1[i] != str2[i]) {
            count++;
            if (count > d) return false;
        }
    }
    return count == d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file) {
        cerr << "Cannot open file: " << file_name << endl;
        exit(EXIT_FAILURE);
    }
    string word;
    while (file >> word) {
        word_list.insert(word);
    }
    file.close();
}

Graph build_graph(const set<string>& word_list) {
    Graph G;
    vector<string> words(word_list.begin(), word_list.end());
    int n = words.size();
    G.numVertices = n;
    G.resize(n);
    map<string, int> word_index;
    for (int i = 0; i < n; i++) {
        word_index[words[i]] = i;
    }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (is_adjacent(words[i], words[j])) {
                G[i].emplace_back(i, j, 1);
                G[j].emplace_back(j, i, 1);
            }
        }
    }
    return G;
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (word_list.find(begin_word) == word_list.end() || word_list.find(end_word) == word_list.end()) {
        error(begin_word, end_word, "Both words must be in the dictionary");
    }
    Graph G = build_graph(word_list);
    vector<string> words(word_list.begin(), word_list.end());
    map<string, int> word_index;
    for (size_t i = 0; i < words.size(); i++) {
        word_index[words[i]] = i;
    }
    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, word_index[begin_word], previous);
    vector<int> path = extract_shortest_path(distances, previous, word_index[end_word]);
    vector<string> word_ladder;
    for (int index : path) {
        word_ladder.push_back(words[index]);
    }
    return word_ladder;
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    for (const auto& word : ladder) {
        cout << word << " ";
    }
    cout << endl;
}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    string begin_word, end_word;
    cout << "Enter start word: ";
    cin >> begin_word;
    cout << "Enter end word: ";
    cin >> end_word;
    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);
    print_word_ladder(ladder);
}
