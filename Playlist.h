using namespace std;

class Playlist {
    string mood;
    string favoriteGenre;
    string favoriteArtist;
    int maxSongs;
public:
    string getMood() {
        return this->mood;
    }
    void setMood(string user_mood) {
        this->mood = user_mood;
    }
};

