using namespace std;

class Playlist {
    string mood;
    string favoriteGenre;
    string favoriteArtist;
    int maxSongs = 0;
    void searchArtist(string user_artist);
public:
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
    void createPlaylist(string mood, int maxSongs);
    void createPlaylistByArtist(string mood, int maxSongs, string artist);
    void createPlaylistByGenre(string mood, int maxSongs, string genre);
};

void Playlist::searchArtist(string user_artist) {
    bool artistFound = false;
    while (!artistFound) {
        // SEARCH FOR ARTIST IN THE CSV FILE ARTIST COLUMN;
        // if artist is found then artistFound = true
        // cout << "Artist successfully found in database!"
        // this->favoriteArtist = user_artist;
        // then exit function

        cout << "Error: Artist not found in database. \n";
        cout << "Please type another artist: \n";
        string favoriteArtist;
        getline(cin, favoriteArtist);
    }


}

