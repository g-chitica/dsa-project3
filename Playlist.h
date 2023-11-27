using namespace std;

class Playlist {
    string mood;
    string favoriteGenre;
    string favoriteArtist;
    int maxSongs = 0;
public:
    string getMood() {
        return this->mood;
    }
    void setMood(string user_mood) {
        this->mood = user_mood;
    }
    void createPlaylist(string mood, int maxSongs);
    void createPlaylistByArtist(string mood, int maxSongs, string artist);
    void createPlaylistByGenre(string mood, int maxSongs, string genre);
};

