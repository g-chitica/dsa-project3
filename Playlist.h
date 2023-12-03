using namespace std;
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
    void merge(vector<Song>& songs, int low, int mid, int high);
    void mergeSort(vector<Song>& songs, int low, int high);
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
            favoriteGenres.emplace_back("alt-rock");
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
            favoriteGenres.emplace_back("dance");
            favoriteGenres.emplace_back("disco");
            favoriteGenres.emplace_back("dub");
            favoriteGenres.emplace_back("dubstep");
            favoriteGenres.emplace_back("edm");
            favoriteGenres.emplace_back("electro");
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
    void createPlaylistByArtistMergeSort(string mood, int maxSongs, string artist);
    void createPlaylistByGenreQuickSort(string mood, int maxSongs, string genre);
    void createPlaylistByGenreMergeSort(int maxSongs);
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
        song1HitRate+=20;
    } else if (this->favoriteArtist != "null" && song1.artist != favoriteArtist && song2.artist == favoriteArtist) {
        song2HitRate+=20;
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
        song2HitRate+=7;
    } else {
        song1HitRate+=7;
    }
    if (song1.dance_ability < song2.dance_ability) {
        song2HitRate++;
    } else {
        song1HitRate++;
    }
    if (song1.energy < song2.energy) {
        song2HitRate++;
    } else {
        song1HitRate++;
    }
    if (song1.loudness < song2.loudness) {
        song2HitRate++;
    } else {
        song1HitRate++;
    }
    if (song1.liveliness < song2.liveliness) {
        song2HitRate++;
    } else {
        song1HitRate++;
    }
    if (song1.valence < song2.valence) {
        song2HitRate++;
    } else {
        song1HitRate++;
    }
    return song1HitRate > song2HitRate;
}

// custom quicksort algorithm based on weighted factors
void Playlist::quickSort(vector<Song>& songs, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(songs, low, high);
        quickSort(songs, low, pivotIndex - 1);
        quickSort(songs, pivotIndex + 1, high);
    }
}

// partition songs based on quick sort hit factors
int Playlist::partition(vector<Song>& songs, int low, int high) {
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
void Playlist::merge(vector<Song>& songs, int low, int mid, int high) {
    int n1 = mid - low + 1;
    int n2 = high - mid;

    // create temp arrays w num of values of left or right halves
    vector<Song> leftSongs(n1);
    vector<Song> rightSongs(n2);

    // copy data to temporary arrays leftSongs[] and rightSongs[]
    for (int i = 0; i < n1; i++) {
        leftSongs[i] = songs[low + i];
    }
    for (int j = 0; j < n2; j++) {
        rightSongs[j] = songs[mid + 1 + j];
    }

    // merge the temporary arrays back into songs[low..high]
    int i = 0;
    int j = 0;
    int k = low;

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

// recursive function to perform merge sort
void Playlist:: mergeSort(vector<Song>& songs, int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;

        // recursively sort the first and second halves
        mergeSort(songs, low, mid);
        mergeSort(songs, mid + 1, high);

        // merge the sorted halves
        merge(songs, low, mid, high);
    }
}

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
    cout << "num artists songs found: " << numArtistSongs << endl;
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
                cout << "Number of filtered songs to sort: " <<  filteredSongs.size() << endl;
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
                }
            }
        }
    }
    cout << "Number of filtered songs to sort: " <<  filteredSongs.size() << endl;
    // populate userPlaylist with the sorted songs up to maxSongs
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
    }
}

void Playlist::createPlaylistByArtistMergeSort(string mood, int maxSongs, string artist) {
   // same thing as above but with algorithm 2
}

//yujin
void Playlist::createPlaylistByGenreQuickSort(string mood, int maxSongs, string genre) {
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

void Playlist::createPlaylistByGenreMergeSort(int maxSongs) {
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
    int current_size = this->filteredSongs.size();
    cout << "num filtered songs found: " << current_size << endl;
    // merge sort the filtered songs found
    mergeSort(this->filteredSongs, 0, current_size - 1);
    for (int i = 0; i < min(maxSongs, static_cast<int>(this->filteredSongs.size())); i++) {
        userPlaylist.push_back(this->filteredSongs[i]);
    }

    // print the sorted playlist
    printPlaylist();

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

