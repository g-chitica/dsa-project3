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
        this->favoriteArtist = user_artist;
        searchArtist(user_artist);
    }
    void createPlaylist(string mood, int maxSongs);
    void createPlaylistByArtist(string mood, int maxSongs, string artist);
    void createPlaylistByGenre(string mood, int maxSongs, string genre);
};

void Playlist::searchArtist(string user_artist) {
    ///FIXME: SEARCH FOR ARTIST IN THE CSV FILE ARTIST COLUMN;
    bool artistFound = false;
    if (artistFound == true) {
        cout << "Artist successfully found in database\n";
    }
    else {
        cout << "Error: Artist not found in database. \n";
        ///FIXME maybe a loop if artist not found but that might mean we have to take the searchArtist() funciton
        ///to public to return a boolean
    }

}

