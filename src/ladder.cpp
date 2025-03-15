#include "ladder.h" // Assuming this is the header file name

void error(string word1, string word2, string msg) {
    cout << "Error finding word ladder from \"" << word1 << "\" to \"" << word2 << "\": " << msg << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    // If lengths differ by more than d, edit distance exceeds d
    if (abs((int)str1.length() - (int)str2.length()) > d) {
        return false;
    }
    
    // Dynamic programming approach for edit distance
    int m = str1.length();
    int n = str2.length();
    
    // Create a table to store results of subproblems
    vector<vector<int>> dp(m + 1, vector<int>(n + 1));
    
    // Fill dp[][] in bottom up manner
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            // If first string is empty, only option is to
            // insert all characters of second string
            if (i == 0)
                dp[i][j] = j;
            
            // If second string is empty, only option is to
            // delete all characters of first string
            else if (j == 0)
                dp[i][j] = i;
            
            // If last characters are same, ignore last char
            // and recur for remaining string
            else if (tolower(str1[i-1]) == tolower(str2[j-1]))
                dp[i][j] = dp[i-1][j-1];
            
            // If the last character is different, consider all
            // possibilities and find the minimum
            else
                dp[i][j] = 1 + min(min(dp[i][j-1],  // Insert
                                       dp[i-1][j]),  // Remove
                                   dp[i-1][j-1]); // Replace
        }
    }
    
    // Return true if edit distance is within d
    return dp[m][n] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    // Two words are adjacent if their edit distance is exactly 1
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    // If start and end words are the same, return just that word
    if (begin_word == end_word) {
        return {begin_word};
    }
    
    // Using a queue to implement BFS
    queue<vector<string>> ladder_queue;
    
    // Start with the begin word
    ladder_queue.push({begin_word});
    
    // Keep track of visited words to avoid cycles and redundant paths
    set<string> visited;
    visited.insert(begin_word);
    
    while (!ladder_queue.empty()) {
        // Get the current ladder from the queue
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        
        // Get the last word in the current ladder
        string last_word = ladder.back();
        
        // Check each word in the dictionary
        for (const string& word : word_list) {
            // Skip if the word has already been visited
            if (visited.find(word) != visited.end()) {
                continue;
            }
            
            // Check if the current dictionary word is adjacent to the last word in the ladder
            if (is_adjacent(last_word, word)) {
                // Create a new ladder by adding the current word
                vector<string> new_ladder = ladder;
                new_ladder.push_back(word);
                
                // If we've reached the end word, return the ladder
                if (word == end_word) {
                    return new_ladder;
                }
                
                // Mark the word as visited and add the new ladder to the queue
                visited.insert(word);
                ladder_queue.push(new_ladder);
            }
        }
    }
    
    // If no ladder is found, return an empty vector
    return {};
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream word_file(file_name);
    string word;
    
    // Check if file was opened successfully
    if (!word_file.is_open()) {
        cerr << "Error: Unable to open file " << file_name << endl;
        return;
    }
    
    // Read words from file and add to set
    while (word_file >> word) {
        // Convert to lowercase
        for (char& c : word) {
            c = tolower(c);
        }
        word_list.insert(word);
    }
    
    word_file.close();
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    
    // Print the ladder with arrows between words
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i < ladder.size() - 1) {
            cout << " → ";
        }
    }
    cout << endl;
    
    // Print the length of the ladder
    cout << "Ladder length: " << ladder.size() << endl;
}

void verify_word_ladder() {
    set<string> word_list;
    string filename = "words.txt";
    
    // Load the dictionary of words
    load_words(word_list, filename);
    
    if (word_list.empty()) {
        cerr << "Error: Dictionary is empty. Make sure the file exists and is properly formatted." << endl;
        return;
    }
    
    string begin_word, end_word;
    
    // Get input from user
    cout << "Enter start word: ";
    cin >> begin_word;
    
    cout << "Enter end word: ";
    cin >> end_word;
    
    // Convert to lowercase for consistency
    for (char& c : begin_word) c = tolower(c);
    for (char& c : end_word) c = tolower(c);
    
    // Check if start and end words are the same
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start and end words are the same");
        return;
    }
    
    // Check if end word is in the dictionary
    if (word_list.find(end_word) == word_list.end()) {
        error(begin_word, end_word, "End word not found in dictionary");
        return;
    }
    
    // Generate the word ladder
    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);
    
    // Print the result
    if (ladder.empty()) {
        error(begin_word, end_word, "No word ladder found");
    } else {
        print_word_ladder(ladder);
    }
}