#include "Song.h"


Song::Song(const std::string& title, const std::string& artist, const std::string& album, short releaseYear, Genre genre) {
    this->title = title;
    this->artist = artist;
    this->album = album;
    this->releaseYear = releaseYear;
    this->genre = genre;
    
}

Genre Song::stringTogenre(const std::string& genre)
{
    if (genre == "Pop")
        return Genre::Pop;
    if (genre == "Rock")
        return Genre::Rock;
    if (genre == "RnB")
        return Genre::RnB;
    if (genre == "Rap")
        return Genre::Rap;
    return Genre::None;

    
}

std::string Song::genreToString(Genre genre)
{
    switch (genre)
    {
    case Genre::Pop: return "Pop";
        break;
    case Genre::Rock: return "Rock";
        break;
    case Genre::RnB: return "RnB";
        break;
    case Genre::Rap: return "Rap";
        break;
    case Genre::None: return "None";
        break;
    default: std::cout<<"Unhandled genre!";
        break;
    }
}

void Song::serializeSong(std::ofstream& songsFile)
{
        songsFile << title << '\n';
        songsFile << artist << '\n';
        songsFile << album << '\n';
        songsFile << releaseYear << '\n';
        songsFile << genreToString(genre) << '\n';
        for (auto rating : ratings )
        {
            songsFile << rating.first << ' ' << rating.second << ' ';
        }
        songsFile << "\n";
}

std::string Song::getTitle() const {
    return title;
}
std::string Song::getArtist() const {
    return artist;
}
std::string Song::getAlbum() const {
    return album;
}
short Song::getReleaseYear() const {
    return releaseYear;
}
float Song::getAverageRating() const
{
    if (ratings.empty()) {
        return 0;
    }
    float combinedRating = 0;
    for (auto itr : ratings) {
        combinedRating += itr.second;
    }

    return combinedRating/ratings.size();
}
Genre Song::getGenre() const {
    return genre;
}

void Song::addRatingsDeserialize(std::map<std::string, float> ratings)
{
    this->ratings = ratings;
}

void Song::rateThisSong(const std::string& username, float rating) {
    if (ratings.count(username) > 0) {
        std::cout << "Error: " << username << " has already voted." << std::endl;
        return;
    }
    // registering the vote
    ratings[username] = rating;
    std::cout << "Your vote has been registered." << std::endl;
}

