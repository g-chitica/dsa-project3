#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Playlist.h"

using namespace std;

// function to read in csv by its delimiters
// citation: https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
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
    // maybe a class for initializing the user playlist?
    cout << "Loading song database... Please wait up to 30 seconds to begin!\n";
    Playlist MyPlaylist;
    cout << "Thank you for your patience...\n";
    cout << endl;
    cout << "Hello! This is a Mood Playlist Generator!\n";
    cout << "Let's begin!...\n";

    bool correctMood = false;

    while (!correctMood) {
        cout << "Please describe in a sentence how you feel: \n";

        string userInput;
        getline(cin, userInput);

        // read in the user input & determine mood based on keyword matches
        vector<string> userWords = parseInput(userInput, ' ');
        string determinedMood = determineMood(moodKeywords, userWords);
        // output determined mood, might need error handling in case it doesnt work
        cout << "We think your mood is... " << determinedMood << "...\n";

        MyPlaylist.setMood(determinedMood);

        // ask the user if the determined mood is correct
        string userResponse;
        cout << "Is this mood correct? (yes/no): \n";
        getline(cin, userResponse);

        // check user response and update correctMood accordingly
        if (userResponse == "yes" || userResponse == "Yes") {
            correctMood = true;
        } else {
            correctMood = false;
            cout << "Let's try again.\n";
        }
    }

    cout << "Choose from one of the following choices to help create your unique playlist!\n";
    cout << "1. Choose favorite genre\n";
    cout << "2. Choose favorite artist\n";
    // maybe another option? we can decide later if it gets too complicated then maybe not
    cout << "3. Exit Program\n";

    string userOption;
    getline(cin, userOption);

    if (userOption == "1") {
        cout << "What is your favorite genre?\n";
        // options should be between rap/hip-hop, rock, pop, edm, international, instrumental, indie, country
        cout << "1. Rap/Hip-Hop\n";
        cout << "2. Rock\n";
        cout << "3. Pop\n";
        cout << "4. EDM\n";
        cout << "5. International\n";
        cout << "6. Instrumental\n";
        cout << "7. Indie\n";
        cout << "8. Country\n";
        string favoriteGenre;
        getline(cin, favoriteGenre);
    }
    if (userOption == "2") {
        cout << "Who is your favorite artist?\n";
        string favoriteArtist;
        getline(cin, favoriteArtist);
        MyPlaylist.setfavoriteArtist(favoriteArtist);
    }
    if (userOption == "3") {
        return 0;
    }

    return 0;
}


