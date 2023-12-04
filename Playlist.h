using namespace std;
using namespace std::chrono;
struct Song {
    Song() {}
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
    vector<string> favoriteGenres;
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
    void merge(vector<Song>& songs, int left, int mid, int right);
    void mergeSort(vector<Song>& songs, int left, int right);
    double stdDanceAbility(string lowerOrUpper);
    double stdEnergy(string lowerOrUpper);
    double stdLoudness(string lowerOrUpper);
    double stdLiveliness(string lowerOrUpper);
    double stdValence(string lowerOrUpper);
    double stdTempo(string lowerOrUpper);

public:
    Playlist() {
        mood = "null";
        favoriteArtist = "null";
        favoriteGenre = "null";
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
            if (parsedInput.size() == 20) {
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
    void setFavoriteArtist(string user_artist) {
        searchArtist(user_artist);
    }
    string getFavoriteArtist() {
        return this->favoriteArtist;
    }
    void setFavoriteGenre(string genreNum) {
        if (genreNum == "1") {
            this->favoriteGenre = "Hip-Hop";
            favoriteGenres.emplace_back("hip-hop");
        }
        if (genreNum == "2") {
            this->favoriteGenre = "Rock";
            favoriteGenres.emplace_back("black-metal");
            favoriteGenres.emplace_back("emo");
            favoriteGenres.emplace_back("death-metal");
            favoriteGenres.emplace_back("goth");
            favoriteGenres.emplace_back("hardcore");
            favoriteGenres.emplace_back("hard-rock");
            favoriteGenres.emplace_back("heavy-metal");
            favoriteGenres.emplace_back("metal");
            favoriteGenres.emplace_back("metalcore");
            favoriteGenres.emplace_back("pysch-rock");
            favoriteGenres.emplace_back("punk");
            favoriteGenres.emplace_back("punk-rock");
            favoriteGenres.emplace_back("rock");
            favoriteGenres.emplace_back("rock-n-roll");
        }
        if (genreNum == "3") {
            this->favoriteGenre = "Pop";
            favoriteGenres.emplace_back("pop");
            favoriteGenres.emplace_back("pop-film");
            favoriteGenres.emplace_back("power-pop");
        }
        if (genreNum == "4") {
            this->favoriteGenre = "EDM";
            favoriteGenres.emplace_back("dubstep");
            favoriteGenres.emplace_back("edm");
            favoriteGenres.emplace_back("electronic");
            favoriteGenres.emplace_back("techno");

        }
        if (genreNum == "5") {
            this->favoriteGenre = "International";
            favoriteGenres.emplace_back("afrobeat");
            favoriteGenres.emplace_back("indian");
            favoriteGenres.emplace_back("k-pop");
            favoriteGenres.emplace_back("salsa");
            favoriteGenres.emplace_back("samba");
            favoriteGenres.emplace_back("spanish");
            favoriteGenres.emplace_back("swedish");
            favoriteGenres.emplace_back("french");
        }
        if (genreNum == "6") {
            this->favoriteGenre = "Instrumental";
            favoriteGenres.emplace_back("classical");
            favoriteGenres.emplace_back("opera");
            favoriteGenres.emplace_back("piano");
        }
        if (genreNum == "7") {
            this->favoriteGenre = "Indie";
            favoriteGenres.emplace_back("alt-rock");
            favoriteGenres.emplace_back("chill");
            favoriteGenres.emplace_back("indie-pop");
            favoriteGenres.emplace_back("garage");
        }
        if (genreNum == "8") {
            this->favoriteGenre = "Country";
            favoriteGenres.emplace_back("country");
        }

    };
    void createPlaylistByArtist(int maxSongs, string artist, int sortType);
    void createPlaylistByGenre(int maxSongs, int sortType);
};

void Playlist::searchArtist(string user_artist) {

    //  cout << songDatabase.size() << endl; commented out this line of code because the # of artist is not need yet
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
        setFavoriteArtist(newFavoriteArtist);
    }
}

// gabby
void Playlist::printPlaylist() {
    if (userPlaylist.empty()) {
        cout << "Your playlist is empty. Please start again with new criteria :(.\n";
    } else {
        cout << "Out of " << songDatabase.size() << " songs...\n";
        cout << "We've created this playlist just for you!\n";
        if (this->favoriteArtist != "null") {
            cout << this->favoriteArtist << " " << this->mood << " mix playlist: \n";
        }
        if (this->favoriteGenre != "null") {
            cout << this->favoriteGenre << " " << this->mood << " mix playlist: \n";
        }
        cout << endl;
        for (int i = 0; i < min(static_cast<int>(userPlaylist.size()), this->maxSongs); i++) {
            cout << i + 1 << ". " << userPlaylist[i].song_name << " - " << userPlaylist[i].artist << "\n";
        }
    }
}
bool Playlist::compareSong(const Song& song1, const Song& song2) {
    // create weights for each song!
    int song1HitRate = 0;
    int song2HitRate = 0;

    if (this->favoriteArtist != "null" && song1.artist == favoriteArtist && song2.artist != favoriteArtist) {
        // made artist comparisons weighted more!
        song1HitRate+=100;
    } else if (this->favoriteArtist != "null" && song1.artist != favoriteArtist && song2.artist == favoriteArtist) {
        song2HitRate+=100;
    }
    if (this->favoriteGenre != "null" && find(favoriteGenres.begin(), favoriteGenres.end(), song1.genre)!= favoriteGenres.end()
    && find(favoriteGenres.begin(), favoriteGenres.end(), song2.genre)== favoriteGenres.end() ) {
        // made genre comparisons weighted more!
        song1HitRate+=20;
    } else if (this->favoriteGenre != "null" && find(favoriteGenres.begin(), favoriteGenres.end(), song1.genre)== favoriteGenres.end()
    && find(favoriteGenres.begin(), favoriteGenres.end(), song2.genre)!= favoriteGenres.end() ) {
        song2HitRate+=20;
    }
    if (song1.popularity < song2.popularity) {
        // song with the highest popularity gets higher score
        song2HitRate+=song2.popularity;
    } else {
        song1HitRate+=song1.popularity;
    }
    if (song1.dance_ability < song2.dance_ability) {
        song2HitRate+=song2.dance_ability;
    } else {
        song1HitRate+=song1.dance_ability;
    }
    if (song1.energy < song2.energy) {
        song2HitRate+=song2.energy;
    } else {
        song1HitRate+=song1.energy;
    }
    if (song1.loudness < song2.loudness) {
        song2HitRate+=song2.loudness;
    } else {
        song1HitRate+=song2.loudness;
    }
    if (song1.liveliness < song2.liveliness) {
        song2HitRate+=song2.liveliness;
    } else {
        song1HitRate+=song2.liveliness;
    }
    if (song1.valence < song2.valence) {
        song2HitRate+=song2.valence;
    } else {
        song1HitRate+=song1.valence;
    }
    return song1HitRate > song2HitRate;
}

// quick sort citation: aman's sorting lecture slides pg. 122
void Playlist::quickSort(vector<Song>& songs, int low, int high) {
    if (low < high) {
        int pivot = partition(songs, low, high);
        quickSort(songs, low, pivot - 1);
        quickSort(songs, pivot + 1, high);
    }
}

// partition songs based on quick sort hit factors
// quick sort citation: aman's sorting lecture slides pg. 122
int Playlist::partition(vector<Song>& songs, int low, int high) {
    // Select the pivot element
    Song pivot = songs[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (compareSong(songs[j], pivot)) {
            i++;
            swap(songs[i], songs[j]);
        }
    }
    swap(songs[i + 1], songs[high]);
    return i + 1;
}
// merge sort citation: aman's sorting lecture slides pg. 90
void Playlist::merge(vector<Song>& songs, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    // create temp vectors w num of values of left or right halves
    vector<Song> leftSongs(n1), rightSongs(n2);
    // copy data back to temporary vectors
    for (int i = 0; i < n1; i++) {
        leftSongs[i] = songs[left + i];
    }
    for (int j = 0; j < n2; j++) {
        rightSongs[j] = songs[mid + 1 + j];
    }

    // merge the left and right vectors into vector
    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (compareSong(leftSongs[i], rightSongs[j])) {
            songs[k] = leftSongs[i];
            i++;
        } else {
            songs[k] = rightSongs[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        songs[k] = leftSongs[i];
        i++;
        k++;
    }
    while (j < n2) {
        songs[k] = rightSongs[j];
        j++;
        k++;
    }
}

// merge sort citation: aman's sorting lecture slides pg. 91
void Playlist:: mergeSort(vector<Song>& songs, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        // recursively sort for the first and second half of the vectors
        mergeSort(songs, left, mid);
        mergeSort(songs, mid + 1, right);

        // merge the sorted sub-vectors
        merge(songs, left, mid, right);
    }
}

// citation for getting time: https://www.geeksforgeeks.org/measure-execution-time-function-cpp/#
void Playlist::createPlaylistByArtist(int maxSongs, string artist, int sortType) {
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
    if (filteredSongs.empty() || current_size != maxSongs) {
        for (const Song& song : songDatabase) {
            try {
                if (uniqueSongs.find(song.song_name) == uniqueSongs.end() &&
                    song.popularity <= filteredSongs[0].popularity + 10 &&
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
                    song.dance_ability >= stdDanceAbility(lower) && song.dance_ability <= stdDanceAbility(upper) &&
                    song.energy >= stdEnergy(lower) && song.energy <= stdEnergy(upper) &&
                    song.loudness >= stdLoudness(lower)&& song.loudness <= stdLoudness(upper) &&
                    song.liveliness >= stdLiveliness(lower) && song.liveliness <= stdLiveliness(upper) &&
                    song.valence >= stdValence(lower) && song.valence <= stdValence(upper) &&
                    song.tempo >= stdTempo(lower) && song.tempo <= stdTempo(upper)) {
                    this->filteredSongs.push_back(song);
                    this->uniqueSongs.insert(song.song_name);
                }
                // cout << "Number of filtered songs to sort: " <<  filteredSongs.size() << endl;
                if (sortType == 1) {
                    quickSort(this->filteredSongs, 0, filteredSongs.size() - 1);
                    for (int i = 0; i < min(maxSongs, static_cast<int>(this->filteredSongs.size())); i++) {
                        userPlaylist.push_back(this->filteredSongs[i]);
                    }
                    printPlaylist();
                }
                else if (sortType == 2) {
                    mergeSort(this->filteredSongs, 0, filteredSongs.size() - 1);
                    for (int i = 0; i < min(maxSongs, static_cast<int>(this->filteredSongs.size())); i++) {
                        userPlaylist.push_back(this->filteredSongs[i]);
                    }
                    printPlaylist();
                    break;
                }
            }
        }
    }
    if (sortType == 1) {
        auto start = high_resolution_clock::now();
        quickSort(this->filteredSongs, 0, filteredSongs.size() - 1);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        for (int i = 0; i < min(maxSongs, static_cast<int>(this->filteredSongs.size())); i++) {
            userPlaylist.push_back(this->filteredSongs[i]);
        }
        printPlaylist();
        cout << "quicksort runtime: ";
        cout << duration.count() << " microseconds" << endl;
    }
    else if (sortType == 2) {
        auto start = high_resolution_clock::now();
        mergeSort(this->filteredSongs, 0, filteredSongs.size() - 1);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        for (int i = 0; i < min(maxSongs, static_cast<int>(this->filteredSongs.size())); i++) {
            userPlaylist.push_back(this->filteredSongs[i]);
        }
        printPlaylist();
        cout << "mergesort runtime: ";
        cout << duration.count() << " microseconds" << endl;
    }
}

void Playlist::createPlaylistByGenre(int maxSongs, int sortType) {
    string lower = "lower";
    string upper = "upper";
    this->maxSongs = maxSongs;
   // same as above but with alg 2
    for (const Song& song : songDatabase) {
        if (find(favoriteGenres.begin(), favoriteGenres.end(), song.genre)!= favoriteGenres.end() &&
            song.dance_ability >= stdDanceAbility(lower) && song.dance_ability <= stdDanceAbility(upper) &&
            song.energy >= stdEnergy(lower) && song.energy <= stdEnergy(upper) &&
            song.loudness >= stdLoudness(lower)&& song.loudness <= stdLoudness(upper) &&
            song.liveliness >= stdLiveliness(lower) && song.liveliness <= stdLiveliness(upper) &&
            song.valence >= stdValence(lower) && song.valence <= stdValence(upper) &&
            song.tempo >= stdTempo(lower) && song.tempo <= stdTempo(upper)) {
            filteredSongs.push_back(song);
            uniqueSongs.insert(song.song_name);
        }
    }
    // merge sort the filtered songs found
    if (sortType == 1) {
        auto start = high_resolution_clock::now();
        quickSort(this->filteredSongs, 0, filteredSongs.size() - 1);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        for (int i = 0; i < min(maxSongs, static_cast<int>(this->filteredSongs.size())); i++) {
            userPlaylist.push_back(this->filteredSongs[i]);
        }
        printPlaylist();
        cout << "quick runtime: ";
        cout << duration.count()  << " microseconds" << endl;
    }
    else if (sortType == 2) {
        auto start = high_resolution_clock::now();
        mergeSort(this->filteredSongs, 0, filteredSongs.size() - 1);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        for (int i = 0; i < min(maxSongs, static_cast<int>(this->filteredSongs.size())); i++) {
            userPlaylist.push_back(this->filteredSongs[i]);
        }
        printPlaylist();
        cout << "mergesort runtime: ";
        cout << duration.count()  << " microseconds" << endl;
    }
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
        if (lowerOrUpper == "lower") return 0.6;
        else {return 0.9;}
    }
    if (this->mood == "energetic"){
        if (lowerOrUpper == "lower") return 0.6;
        else {return 1.0;}
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
        if (lowerOrUpper == "lower") { return 0.4;}
        else { return 0.8;}
    }
    if (this->mood == "energetic"){
        if (lowerOrUpper == "lower") { return 0.7;}
        else { return 1.0;}
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
        if (lowerOrUpper == "lower") return -7.0;
        else {return 0.0;}
    }
    if (this->mood == "energetic"){
        if (lowerOrUpper == "lower") return -6.0;
        else {return 0.0;}
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
        if (lowerOrUpper == "lower") {return 0.1;}
        else {return 0.4;}
    }
    if (this->mood == "energetic"){
        if (lowerOrUpper == "lower") {return 0.0;}
        else {return 1.0;}
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
        if (lowerOrUpper == "lower") { return 0.5;}
        else {return 0.8;}
    }
    if (this->mood == "energetic"){
        if (lowerOrUpper == "lower") { return 0.6;}
        else {return 1.0;}
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
        if (lowerOrUpper == "lower") {return 60.0;}
        else {return 90.0;}
    }
    if (this->mood == "energetic"){
        if (lowerOrUpper == "lower") { return 110.0;}
        else {return 220.0;}
    }
    return 0;
}

