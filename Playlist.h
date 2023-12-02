using namespace std;
struct Song {
    string artist;
    string song_name;
    string genre;
    double dance_ability;
    double energy;
    double loudness;
    double liveliness;
    double valence;
    double tempo;
    Song(string artist, string song_name, string genre, double dance_ability, double energy,
         double loudness, double liveliness, double valence, double tempo)
            : artist(artist), song_name(song_name), genre(genre),
              dance_ability(dance_ability), energy(energy), loudness(loudness),
              liveliness(liveliness), valence(valence), tempo(tempo) {}
};
class Playlist {
    string mood;
    string favoriteGenre;
    string favoriteArtist;
    int maxSongs = 0;
    void searchArtist(string user_artist);
    vector<Song> songDatabase; // vector to store all the songs in the csv file as song objects.
    vector<Song> userPlaylist; // vector to store only the user's playlist songs
public:
    Playlist() {
        mood = "null";
        favoriteGenre = "null";
        favoriteArtist = "null";
        maxSongs = 0;
        // read the spotify_songs csv file and put them in the songDatabase
        ifstream file("spotify_data.csv");
        string line;
        getline(file, line); // skip the header
        while (getline(file, line)) {
            stringstream ss(line);
            string file_col;
            vector<string> parsedInput;

            // split the line into their columns using ',' as delimiter
            while (getline(ss, file_col, ',')) {
                parsedInput.push_back(file_col);
            }

            // ensure that there are enough columns in the line
            if (parsedInput.size() >= 20) {
                try {
                    // create a Song object and add it to the songDatabase
                    // make sure case matches though:
                    // citation https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/#
                    transform(parsedInput[1].begin(), parsedInput[1].end(), parsedInput[1].begin(), ::tolower);

                    Song DBSong(parsedInput[1], parsedInput[2], parsedInput[6],
                            stod(parsedInput[7]), stod(parsedInput[8]), stod(parsedInput[10]),
                                stod(parsedInput[15]), stod(parsedInput[16]), stod(parsedInput[17]));

                    songDatabase.push_back(DBSong);
                } catch (const invalid_argument& e) {
                    // catch any errors that may occur due to invalid data in the csv file
                }
            }
        }

        file.close();
    }
    string getMood() {
        return this->mood;
    }
    void setMood(string user_mood) {
        this->mood = user_mood;
    }
    void setfavoriteGenre(string user_genre){
        this->favoriteGenre = user_genre;
    }
    void setfavoriteArtist(string user_artist) {
        searchArtist(user_artist);
    }
    void createPlaylistByArtistAlg1(string mood, int maxSongs, string artist);
    void createPlaylistByArtistAlg2(string mood, int maxSongs, string artist);
    void createPlaylistByGenreAlg1(string mood, int maxSongs, string genre);
    void createPlaylistByGenreAlg2(string mood, int maxSongs, string genre);
};

void Playlist::searchArtist(string user_artist) {
    cout << songDatabase.size() << endl;
    bool artistFound = false;

    for (Song &song : songDatabase) {
        if (song.artist == user_artist) {
            artistFound = true;
            break;
        }
    }

    if (artistFound) {
        // if artist is found then make the users favorite artist that artist
        cout << "Artist successfully found in the database!\n";
        this->favoriteArtist = user_artist;
    } else {
        // if not continue to search for a different artist!
        cout << "Error: Artist '" + user_artist + "' was not found in the database.\n";
        cout << "Please type another artist: \n";
        string newFavoriteArtist;
        getline(cin, newFavoriteArtist);
        setfavoriteArtist(newFavoriteArtist);
    }
}

// gabby
void Playlist::createPlaylistByArtistAlg1(string mood, int maxSongs, string artist) {
 // if user mood "sad"
 if (mood == "sad") {
     for (int i = 0; i < maxSongs; i++) {
         for (Song &song : songDatabase) {

/*
             search for the artist songs that fit criteria of:
             Danceability: 0.2-0.6
             Energy: 0.2-0.6
             Loudness: -20.0 to -8.0
             Mode: 0 (Minor)
             Speechiness: 0.03-0.2
             Instrumentalness: 0.4-0.9
             Liveness: 0.1-0.3
             Valence: 0.0-0.4
             Tempo: 60-100 */
             // if no songs found by that artist that fit criteria then just take off the criteria and add songs that do
             // from the whole database
             // userPlaylist.pushback(song);
         }
     }
 }
}

void Playlist::createPlaylistByArtistAlg2(string mood, int maxSongs, string artist) {
   // same thing as above but with algorithm 2
}

void Playlist::createPlaylistByGenreAlg1(string mood, int maxSongs, string genre) {
    // search by genre
}

void Playlist::createPlaylistByGenreAlg2(string mood, int maxSongs, string genre) {
   // same as above but with alg 2
}

