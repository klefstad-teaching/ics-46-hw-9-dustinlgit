#include "ladder.h"

void error(string word1, string word2, string msg) {
    cout << "Error between words: '" << word1 << "' and '" << word2 << "': " << msg << endl;
}

// Check if two words differ by more than 'd' edits
bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    int m = str1.length();
    int n = str2.length();
    
    vector<vector<int>> dp(m + 1, vector<int>(n + 1));
    
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0) dp[i][j] = j;
            else if (j == 0) dp[i][j] = i;
            else if (str1[i - 1] == str2[j - 1]) dp[i][j] = dp[i - 1][j - 1];
            else dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
        }
    }

    return dp[m][n] <= d;
}

// Check if two words differ by exactly one letter
bool is_adjacent(const string& word1, const string& word2) {
    if (word1.length() != word2.length()) return false;
    int count = 0;
    for (size_t i = 0; i < word1.length(); i++) {
        if (word1[i] != word2[i]) {
            count++;
            if (count > 1) return false;
        }
    }
    return count == 1;
}

// Load words from file into set
void load_words(set<string> & word_list, const string& file_name) {
    ifstream file(file_name);
    string word;
    while (file >> word) {
        word_list.insert(word);
    }
    file.close();
}

// Print word ladder
void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found!" << endl;
        return;
    }
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i < ladder.size() - 1) cout << " -> ";
    }
    cout << endl;
}

// Verify word ladder
void verify_word_ladder() {
    // This function should verify if the word ladder is valid.
    // It will ensure that each word differs from the previous one by exactly one letter.
    // If the ladder is not valid, an error message will be printed.
}

// Generate word ladder using BFS
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    queue<vector<string>> q;
    set<string> visited;

    q.push({begin_word});
    visited.insert(begin_word);

    while (!q.empty()) {
        vector<string> path = q.front();
        q.pop();
        string last_word = path.back();

        // If we reached the target word, return the path
        if (last_word == end_word) return path;

        // Try changing each letter in the word
        for (size_t i = 0; i < last_word.size(); i++) {
            string new_word = last_word;
            for (char c = 'a'; c <= 'z'; c++) {
                new_word[i] = c;

                // If it's a valid transformation and not visited
                if (word_list.find(new_word) != word_list.end() && visited.find(new_word) == visited.end() && is_adjacent(last_word, new_word)) {
                    visited.insert(new_word);
                    vector<string> new_path = path;
                    new_path.push_back(new_word);
                    q.push(new_path);
                }
            }
        }
    }

    return {}; // No ladder found
}