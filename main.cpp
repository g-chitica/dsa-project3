#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>
#include <unordered_set>
#include <random>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Playlist.h"

using namespace std;


// function to read in csv by its delimiter
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
        if (!determinedMood.empty()) {
            cout << "We think your mood is... " << determinedMood << "...\n";
            MyPlaylist.setMood(determinedMood);
        }

        // if user did not provide valid input, then re-prompt user for input until it's valid
        do {
            if (determinedMood.empty()) {
                cout << "We didn't catch that, please try again. \n";

                string userInputRedo;
                getline(cin, userInputRedo);

                vector<string> userWordsRedo = parseInput(userInputRedo, ' ');
                determinedMood = determineMood(moodKeywords, userWordsRedo);

                if (!determinedMood.empty()) {
                    cout << "We think your mood is... " << determinedMood << "...\n";
                    MyPlaylist.setMood(determinedMood);
                }
            }
        } while (determinedMood.empty());

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
        // options should be between these...
        // most of the genres in the csv file can fall under these general genres.
        cout << "1. Rap/Hip-Hop\n"; // inc. hip-hop
        cout << "2. Rock\n"; // inc. alt-rock, black-metal, death-metal, emo, goth, hardcore, hard-rock, heavy-metal,
        // metal, metalcore, psych-rock, punk, punk-rock, rock, rock-n-roll
        cout << "3. Pop\n"; // inc. pop, pop-film, power-pop
        cout << "4. EDM\n"; // inc. dance, disco, dub, dubstep, edm , electro, electronic, techno
        cout << "5. International\n"; // inc. afrobeat, indian, k-pop, salsa, samba, spanish, swedish, french
        cout << "6. Instrumental\n"; // inc. classical, opera, piano
        cout << "7. Indie\n"; // inc. chill, indie-pop, garage
        cout << "8. Country\n"; // inc. country
        string favoriteGenre;
        getline(cin, favoriteGenre);
        MyPlaylist.setFavoriteGenre(favoriteGenre);
        int num_songs;
        cout << "How many songs would you like in your playlist?\n";
        cin >> num_songs;
        cout << "Finally, which algorithm would you like to sort the playlist?:\n";
        // print statement to easily compare between quick sort and merge sort
        cout << "1. Quick Sort\n";
        cout << "2. Merge Sort \n";
        int sort_type;
        cin >> sort_type;
        // if user enter invalid input of the options, prompt them to reenter input
        if (sort_type != 1 && sort_type != 2) {
            cout << "We did not catch that, please reenter \n";
            cin >> sort_type;
        }

        MyPlaylist.createPlaylistByGenre(num_songs, sort_type);
    }
    else if (userOption == "2") {
        cout << "Who is your favorite artist?\n";
        string favoriteArtist;
        getline(cin, favoriteArtist);
        MyPlaylist.setFavoriteArtist(favoriteArtist);
        int num_songs;
        cout << "How many songs would you like in your playlist?\n";
        cin >> num_songs;
        // create playlist!!
        // print statement to easily compare between quick sort and merge sort
        cout << "Finally, which algorithm would you like to sort the playlist?:\n";
        cout << "1. Quick Sort\n";
        cout << "2. Merge Sort \n";
        int sort_type;
        cin >> sort_type;
        // if user enter invalid input of the options, prompt them to reenter input
        if (sort_type != 1 && sort_type != 2) {
            cout << "We did not catch that, please reenter \n";
            cin >> sort_type;
        }
        MyPlaylist.createPlaylistByArtist(num_songs, MyPlaylist.getFavoriteArtist(), sort_type);
    }
    else if (userOption == "3") {
        return 0;
    }

    return 0;
}


