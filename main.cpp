#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// function to read in csv by its delimiters
vector<string> parseInput(const string& line, char delimiter) {
    vector<string> parsedInput;
    stringstream stream(line);
    string word;
    while (getline(stream, word, delimiter)) {
        parsedInput.push_back(word);
    }
    return parsedInput;
}

string determineMood(unordered_map<string, vector<string>>& moodKeywords, vector<string> userInput) {
    int maxMatches = 0;
    string determinedMood;

    // takes in unordered_map<string, vector<string>> moodKeywords vector userWords
    for (const auto& moodEntry : moodKeywords) {
        const string& mood = moodEntry.first;
        const vector<string>& keywords = moodEntry.second;

        int matches = 0;
        for (const string& words : userInput) {
            if (find(keywords.begin(), keywords.end(), words) != keywords.end()) {
                matches++;
            }
        }

        if (matches > maxMatches) {
            maxMatches = matches;
            determinedMood = mood;
        }
    }
    return determinedMood;

}

unordered_map<string, vector<string>> readMoodFile() {
    // read in the csv file and populate a map of moods and their associated keywords
    ifstream file("moods.csv");
    unordered_map<string, vector<string>> moodKeywords;
    // map: "mood", vector<"mood keywords"
    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        // read in the keywords per line
        vector<string> lineKeywords = parseInput(line, ',');
        string mood = lineKeywords[0];
        // remove the mood and make it the key in the map: "mood", vector<"mood keywords"
        lineKeywords.erase(lineKeywords.begin());
        moodKeywords[mood] = lineKeywords;
    }
    file.close();
    return moodKeywords;
}


int main() {
    // get user input
    unordered_map<string, vector<string>> moodKeywords = readMoodFile();
    // maybe a class for initializing the user playlist
    // Playlist MyPlaylist

    cout << "Hello! This is a Mood Playlist Generator\n" << endl;
    cout << "Please describe in a sentence how you feel\n" << endl;
    // happy, sad/anxious, calm, mad, energetic/dance
    // string mood;
    // mood = based on the determineMood function
    // MyPlaylist.setMood(mood)


    string userInput;
    getline(cin, userInput);

    // read in the user input
    vector<string> userWords = parseInput(userInput, ' ');
    // determine mood based on keyword matches
    string determinedMood = determineMood(moodKeywords, userWords);
    // test output determined mood
    cout << "We think your mood is... " << determinedMood << "...\n";

    return 0;
}
