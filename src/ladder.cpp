#include "ladder.h"
void error(string word1, string word2, string msg) {
    cout << "Error between words: '" << word1 << "' and '" << word2 << "': " << msg << endl;
}

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

bool is_adjacent(const string& word1, const string& word2) {
    if (word1 == word2) {
        return true;
    }

    if (abs(static_cast<int>(word1.length()) - static_cast<int>(word2.length())) == 1) {
        const string& shorter = word1.length() < word2.length() ? word1 : word2;
        const string& longer = word1.length() > word2.length() ? word1 : word2;

        for (size_t i = 0; i < longer.length(); ++i) {
            string temp = longer;
            temp.erase(i, 1);

            if (temp == shorter) {
                return true;
            }
        }
    }

    int diff_count = 0;
    if (word1.length() != word2.length()) {
        return false;
    }

    for (size_t i = 0; i < word1.length(); ++i) {
        if (word1[i] != word2[i]) {
            ++diff_count;
            if (diff_count > 1) {
                return false;
            }
        }
    }
    return diff_count == 1;
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    string word;
    while (file >> word) {
        word_list.insert(word);
    }
    file.close();
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found.\n";
    } else {
        cout << "Word ladder found: ";
        for (const string& word : ladder) {
            cout << word << " ";
        }
        cout << "\n";
    }
}

void verify_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "The ladder is empty!" << endl;
        return;
    }

    for (size_t i = 1; i < ladder.size(); i++) {
        if (!is_adjacent(ladder[i - 1], ladder[i])) {
            cout << "Error: [" << ladder[i - 1] << ", " << ladder[i] << "] not adjacent" << endl;
            return;
        }
    }

    cout << "The word ladder is valid." << endl;
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    // if (begin_word == end_word) {
    //     return {begin_word};
    // }
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});
    set<string> visited;
    visited.insert(begin_word);
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();
        for (const string& word : word_list) {
            if (is_adjacent(last_word, word)) {
                if (visited.find(word) == visited.end()) {
                    visited.insert(word);
                    vector<string> new_ladder = ladder;
                    new_ladder.push_back(word);
                    if (word == end_word) {
                        return new_ladder;
                    }
                    ladder_queue.push(new_ladder);
                }
            }
        }
    }
    return {};
}