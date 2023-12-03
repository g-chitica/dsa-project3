using namespace std;
struct Song {
    string artist;
    string song_name;
    double popularity;
    string genre;
    double dance_ability;
    double energy;
    double loudness;
    double liveliness;
    double valence;
    double tempo;
    Song(string artist, string song_name, double popularity, string genre, double dance_ability, double energy,
         double loudness, double liveliness, double valence, double tempo)
            : artist(artist), song_name(song_name), popularity(popularity), genre(genre),
              dance_ability(dance_ability), energy(energy), loudness(loudness),
              liveliness(liveliness), valence(valence), tempo(tempo) {}
};
class Playlist {
    string mood;
    string favoriteGenre;
    string favoriteArtist;
    int maxSongs = 0;
    double numArtistSongs = 0;
    void searchArtist(string user_artist);
    vector<Song> songDatabase; // vector to store all the songs in the csv file as song objects.
    vector<Song> userPlaylist; // vector to store only the user's playlist songs
    vector<Song> filteredSongs;
    unordered_set<string> uniqueSongs;
    void printPlaylist();
    bool compareSong(const Song& song1, const Song& song2);
    void quickSort(vector<Song>& songs, int low, int high);
    int partition(vector<Song>& songs, int low, int high);
    double stdDanceAbility(string lowerOrUpper);
    double stdEnergy(string lowerOrUpper);
    double stdLoudness(string lowerOrUpper);
    double stdLiveliness(string lowerOrUpper);
    double stdValence(string lowerOrUpper);
    double stdTempo(string lowerOrUpper);

public:
    Playlist() {
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

                    Song DBSong(parsedInput[1], parsedInput[2], stod(parsedInput[4]), parsedInput[6],
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
    void setMood(string user_mood) {
        this->mood = user_mood;
    }
    void setfavoriteArtist(string user_artist) {
        searchArtist(user_artist);
    }
    string getfavoriteArtist() {
        return this->favoriteArtist;
    }
    
    void createPlaylistByArtistQuickSort(int maxSongs, string artist);
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
void Playlist::printPlaylist() {
    if (userPlaylist.empty()) {
        cout << "Your playlist is empty. Please start again with new criteria :(.\n";
    } else {
        // shuffle the playlist order
        random_device rd;
        mt19937 g(rd());
        shuffle(userPlaylist.begin(), userPlaylist.end(), g);

        cout << "Here is your custom playlist:\n";
        for (int i = 0; i < min(static_cast<int>(userPlaylist.size()), this->maxSongs); i++) {
            cout << i + 1 << ". " << userPlaylist[i].song_name << " - " << userPlaylist[i].artist << "\n";
        }
    }
}
bool Playlist::compareSong(const Song &song1, const Song &song2) {
    if (song1.artist == favoriteArtist && song2.artist != favoriteArtist) {
        // song1 comes first because it's by the favorite artist
        return true;
    } else if (song1.artist != favoriteArtist && song2.artist == favoriteArtist) {
        // song2 comes first because it's by the favorite artist
        return false;
    }
    if (song1.dance_ability != song2.dance_ability) {
        return song1.dance_ability < song2.dance_ability;
    }
    if (song1.energy != song2.energy) {
        return song1.energy < song2.energy;
    }
    if (song1.loudness != song2.loudness) {
        return song1.loudness < song2.loudness;
    }
    if (song1.liveliness != song2.liveliness) {
        return song1.liveliness < song2.liveliness;
    }
    if (song1.valence != song2.valence) {
        return song1.valence < song2.valence;
    }
    return song1.tempo < song2.tempo;
}

// partition function for quicksort
int Playlist::partition(vector<Song>& songs, int low, int high) {
    Song pivot = songs[high];

    // find the index of the pivot after rearranging the array
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (compareSong(songs[j], pivot)) {
            i++;
            // swap songs[i] and songs[j]
            swap(songs[i], songs[j]);
        }
    }

    // swap songs[i + 1] and pivot
    swap(songs[i + 1], songs[high]);

    // return the index of the pivot
    return i + 1;
}

// quicksort algorithm
void Playlist::quickSort(vector<Song>& songs, int low, int high) {
    if (low < high) {
        // partition the array and get the pivot index
        int pivotIndex = partition(songs, low, high);

        // recursively sort the sub-arrays
        quickSort(songs, low, pivotIndex - 1);
        quickSort(songs, pivotIndex + 1, high);
    }
}

void Playlist::createPlaylistByArtistQuickSort(int maxSongs, string artist) {
    string lower = "lower";
    string upper = "upper";
    this->maxSongs = maxSongs;
    for (const Song& song : songDatabase) {
        if (song.artist == artist && (numArtistSongs / maxSongs <= 0.4) &&
            song.dance_ability >= stdDanceAbility(lower) && song.dance_ability <= stdDanceAbility(upper) &&
            song.energy >= stdEnergy(lower) && song.energy <= stdEnergy(upper) &&
            song.loudness >= stdLoudness(lower)&& song.loudness <= stdLoudness(upper) &&
            song.liveliness >= stdLiveliness(lower) && song.liveliness <= stdLiveliness(upper) &&
            song.valence >= stdValence(lower) && song.valence <= stdValence(upper) &&
            song.tempo >= stdTempo(lower) && song.tempo <= stdTempo(upper)) {
            filteredSongs.push_back(song);
            uniqueSongs.insert(song.song_name);
            numArtistSongs++;
        }
    }
    int current_size = this->filteredSongs.size();
    cout << "filtered song size: " << current_size;
    // if no songs found, add songs without artist criteria but fit mood from the whole database
    if (filteredSongs.empty() || current_size != maxSongs) {
        for (const Song& song : songDatabase) {
            try {
                if (uniqueSongs.find(song.song_name) == uniqueSongs.end() &&
                    song.artist != artist &&
                    song.popularity >= filteredSongs[1].popularity &&
                    song.dance_ability >= stdDanceAbility(lower) && song.dance_ability <= stdDanceAbility(upper) &&
                    song.energy >= stdEnergy(lower) && song.energy <= stdEnergy(upper) &&
                    song.loudness >= stdLoudness(lower)&& song.loudness <= stdLoudness(upper) &&
                    song.liveliness >= stdLiveliness(lower) && song.liveliness <= stdLiveliness(upper) &&
                    song.valence >= stdValence(lower) && song.valence <= stdValence(upper) &&
                    song.tempo >= stdTempo(lower) && song.tempo <= stdTempo(upper)) {
                    this->filteredSongs.push_back(song);
                    this->uniqueSongs.insert(song.song_name);
                }
            }
            catch (const invalid_argument& e) {
                if (uniqueSongs.find(song.song_name) == uniqueSongs.end() &&
                    song.artist != artist &&
                    song.dance_ability >= stdDanceAbility(lower) && song.dance_ability <= stdDanceAbility(upper) &&
                    song.energy >= stdEnergy(lower) && song.energy <= stdEnergy(upper) &&
                    song.loudness >= stdLoudness(lower)&& song.loudness <= stdLoudness(upper) &&
                    song.liveliness >= stdLiveliness(lower) && song.liveliness <= stdLiveliness(upper) &&
                    song.valence >= stdValence(lower) && song.valence <= stdValence(upper) &&
                    song.tempo >= stdTempo(lower) && song.tempo <= stdTempo(upper)) {
                    this->filteredSongs.push_back(song);
                    this->uniqueSongs.insert(song.song_name);
                }
                quickSort(this->filteredSongs, 0, filteredSongs.size() - 1);
                // populate userPlaylist with the sorted songs up to maxSongs
                for (int i = 0; i < min(maxSongs, static_cast<int>(this->filteredSongs.size())); i++) {
                    userPlaylist.push_back(this->filteredSongs[i]);
                }
            }
        }
    }
    quickSort(this->filteredSongs, 0, filteredSongs.size() - 1);
    // populate userPlaylist with the sorted songs up to maxSongs
    for (int i = 0; i < min(maxSongs, static_cast<int>(this->filteredSongs.size())); i++) {
        userPlaylist.push_back(this->filteredSongs[i]);
    }
    printPlaylist();
}

void Playlist::createPlaylistByArtistAlg2(string mood, int maxSongs, string artist) {
   // same thing as above but with algorithm 2
}

//yujin
void Playlist::createPlaylistByGenreAlg1(string mood, int maxSongs, string genre) {
    // search by genre
    cout << "favoriteGenre " << favoriteGenre << endl;
    cout << "max songs: " << maxSongs << endl;
    cout << "songDatabase size: " << songDatabase.size() << endl;
    this->maxSongs = maxSongs;
    if(mood == "sad"){
        if (favoriteGenre == "1"){
            for (const Song& song : songDatabase){
                if (song.dance_ability <= 0.2 && song.energy <= 0.2 &&
                    song.loudness >= -20.0 && song.loudness <= -8.0 &&
                    song.liveliness >= 0.0 && song.liveliness <= 0.3 &&
                    song.valence >= 0.0 && song.valence <= 0.4 &&
                    song.tempo >= 60 && song.tempo <= 100) {
                    filteredSongs.push_back(song);
                    uniqueSongs.insert(song.song_name);
                    numArtistSongs++;
                }
            }
        }
        int current_size = this->filteredSongs.size();
        cout << "Filtered Songs Size: " << current_size << endl;
        quickSort(this->filteredSongs, 0, filteredSongs.size() - 1);
        for (int i = 0; i < min(maxSongs, static_cast<int>(this->filteredSongs.size())); i++) {
            userPlaylist.push_back(this->filteredSongs[i]);
        }
        cout << "User Playlist: " << endl;
        for (const auto& song : userPlaylist) {
            cout << song.song_name << " - " << song.artist << endl;
        }
        printPlaylist();
    }
}

void Playlist::createPlaylistByGenreAlg2(string mood, int maxSongs, string genre) {
   // same as above but with alg 2
}

double Playlist::stdDanceAbility(string lowerOrUpper) {
    if (this->mood == "sad") {
        if (lowerOrUpper == "lower") return 0.2;
        else { return 0.6; }
    }
    if (this->mood == "happy") {
        if (lowerOrUpper == "lower") { return 0.5;}
        else { return 1.0;}
    }
    if (this->mood == "calm"){
        if (lowerOrUpper == "lower") return 0.0;
        else {return 0.5;}
    }
    if (this->mood == "mad") {

    }
    if (this->mood == "energetic"){

    }
}

double Playlist::stdEnergy(string lowerOrUpper) {
    if (this->mood == "sad") {
        if (lowerOrUpper == "lower") return 0.2;
        else { return 0.6; }
    }
    if (this->mood == "happy") {
        if (lowerOrUpper == "lower") { return 0.5;}
        else { return 1.0;}
    }
    if (this->mood == "calm"){
        if (lowerOrUpper == "lower") {return 0.3;}
        else {return 0.7;}
    }
    if (this->mood == "mad") {

    }
    if (this->mood == "energetic"){

    }
}

double Playlist::stdLoudness(string lowerOrUpper) {
    if (this->mood == "sad") {
        if (lowerOrUpper == "lower") return -20.0;
        else { return -8.0; }
    }
    if (this->mood == "happy") {
        if (lowerOrUpper == "lower") { return -8.0;}
        else { return -4.0; }
    }
    if (this->mood == "calm"){
        if (lowerOrUpper == "lower") return -15.0;
        else {return -6.0;}
    }
    if (this->mood == "mad") {

    }
    if (this->mood == "energetic"){

    }
}

double Playlist::stdLiveliness(string lowerOrUpper) {
    if (this->mood == "sad") {
        if (lowerOrUpper == "lower") return 0.1;
        else {return 0.3;}
    }
    if (this->mood == "happy") {
        if (lowerOrUpper == "lower") {return 0.3;}
        else {return 0.8;}
    }
    if (this->mood == "calm"){
        if (lowerOrUpper == "lower") {return 0.0;}
        else {return 0.4;}

    }
    if (this->mood == "mad") {

    }
    if (this->mood == "energetic"){

    }
    return 0;
}

double Playlist::stdValence(string lowerOrUpper) {
    if (this->mood == "sad") {
        if (lowerOrUpper == "lower") return 0.0;
        else {return 0.4;}
    }
    if (this->mood == "happy") {
        if (lowerOrUpper == "lower") { return 0.5;}
        else {return 1.0;}
    }
    if (this->mood == "calm"){
        if (lowerOrUpper == "lower") return 0.2;
        else {return 0.6;}
    }
    if (this->mood == "mad") {

    }
    if (this->mood == "energetic"){

    }
}

double Playlist::stdTempo(string lowerOrUpper) {
    if (this->mood == "sad") {
        if (lowerOrUpper == "lower") return 50.0;
        else {return 100.0;}
    }
    if (this->mood == "happy") {
        if (lowerOrUpper == "lower") { return 100.0;}
        else {return 200.0;}
    }
    if (this->mood == "calm"){
        if (lowerOrUpper == "lower") {return 60.0;}
        else {return 100.0;}
    }
    if (this->mood == "mad") {

    }
    if (this->mood == "energetic"){

    }
    return 0;
}

