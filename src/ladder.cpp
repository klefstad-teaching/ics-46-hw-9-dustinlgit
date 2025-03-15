#include "ladder.h"

// Error handling function
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

bool is_adjacent(const string& word1, const string& word2) {
    // If the words are identical, they are adjacent in the word ladder.
    if (word1 == word2) {
        return true;
    }

    // If the length of the words differs by exactly one character, check for adjacency by one character addition/removal.
    if (abs(static_cast<int>(word1.length()) - static_cast<int>(word2.length())) == 1) {
        const string& shorter = word1.length() < word2.length() ? word1 : word2;
        const string& longer = word1.length() > word2.length() ? word1 : word2;
        
        // Try removing one character from the longer word to match the shorter word
        for (size_t i = 0; i < longer.length(); ++i) {
            string temp = longer;
            temp.erase(i, 1);  // Remove one character
            if (temp == shorter) {
                return true;  // They are adjacent if removing one character matches the shorter word
            }
        }
    }

    // Otherwise, check for one character difference
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


// Load words from file into set
void load_words(set<string> & word_list, const string& file_name) {
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



// Verify word ladder
void verify_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "The ladder is empty!" << endl;
        return;
    }

    // Check that each consecutive pair of words differs by exactly one letter
    for (size_t i = 1; i < ladder.size(); i++) {
        if (!is_adjacent(ladder[i - 1], ladder[i])) {
            cout << "Error: The words '" << ladder[i - 1] << "' and '" << ladder[i] << "' are not adjacent!" << endl;
            return;
        }
    }

    cout << "The word ladder is valid." << endl;
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    // If the begin word and end word are the same, return immediately with a ladder of size 1.
    if (begin_word == end_word) {
        return {begin_word};
    }

    // BFS initialization
    queue<vector<string>> ladders;  // Queue of word ladders
    set<string> visited;  // Set of visited words
    ladders.push({begin_word});
    visited.insert(begin_word);

    // BFS loop
    while (!ladders.empty()) {
        int level_size = ladders.size();
        
        // Process all ladders at the current level
        for (int i = 0; i < level_size; ++i) {
            vector<string> current_ladder = ladders.front();
            ladders.pop();
            string last_word = current_ladder.back();

            // Try to transform each word in the word list
            for (const string& word : word_list) {
                if (is_adjacent(last_word, word) && visited.find(word) == visited.end()) {
                    // Check if we found the end word
                    if (word == end_word) {
                        current_ladder.push_back(word);
                        return current_ladder;  // Return the complete word ladder
                    }

                    // Add the new word to the ladder and enqueue it for further exploration
                    visited.insert(word);
                    vector<string> new_ladder = current_ladder;
                    new_ladder.push_back(word);
                    ladders.push(new_ladder);
                }
            }
        }
    }
    return {};  // No ladder found
}