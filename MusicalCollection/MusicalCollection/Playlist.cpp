#include "Playlist.h"

Playlist::Playlist()
{
}

Playlist::Playlist(const std::string& name)
{
	this->name = name;
}

Playlist::Playlist(const std::string& name, std::forward_list<const Song*> songs)
{
	this->name = name;
	this->songs = songs;
}

void Playlist::serializePlaylist(std::ofstream& usersFile)
{
	usersFile << name << '\n';
	usersFile << '{' << '\n';
	for (auto song : songs) {
		usersFile << song->getTitle() << '\n';
	}
	usersFile << '}' << '\n';
}

void Playlist::addSongToThisPlaylist(const Song* song) {
	
	songs.push_front(song);
}

void Playlist::printSongs()
{
	std::cout << "Playlist: " << name << std::endl;
	std::cout << "Songs: " << std::endl;
	std::cout << std::endl;
	for (const Song* song : songs) {
		std::cout << "Title: " << song->getTitle()<<std::endl;
		std::cout << "Artist: " << song->getArtist() << std::endl;
		std::cout << "Album: " << song->getAlbum() << std::endl;
		std::cout << "Year of release: " << song->getReleaseYear() << std::endl;
		std::cout << "Genre: " << song->genreToString(song->getGenre()) << std::endl;
		std::cout << "Average rating: " << song->getAverageRating() << std::endl;
		std::cout<<std::endl;
	}
}

std::string Playlist::getName() const
{
	return name;
}
