#pragma once
#include "Playlist.h"
#include<vector>
class User
{
private:
	std::string username;
	std::string pass;
	std::string fullName;
	std::string bDay;
	std::vector<Genre> favoriteGenres; //there won't be more than 4 genres, and if more genres are added they will still be few
	std::map<std::string, Playlist> playlists; //because of searching -> logN
	std::string currPlaylist;
	
public:
	User();
	User(const std::string& username, const std::string& pass, const std::string& fullName, const std::string& bDay, std::vector<Genre> favGenres);

	void serializeUser(std::ofstream& usersFile);

	//change data
	void changeName();
	void changeBday();
	void changePass();
	void addFavGenre();
	void removeFavGenre();

	//add song to playlist
	void addSongToPlaylist(const Song* toAdd, const std::string& nameOfPlaylist); 
	void addSongToCurrPlayList(const Song* songToAdd);

	//get methods
	std::string getUserName() const;
	std::string getPass() const;
	std::vector<Genre> getFavGenres() const;
	std::map<std::string, Playlist> getPlaylists()const;



	bool loadPlaylist(const std::string& nameOfPlaylist); // finds the playlist by name and makes it currPlaylist, returns false if the playlist is not found
	void currPlaylistSongsInfo(); // prints information for the songs from the current playlist

	void addPlaylist(const Playlist& playlist); //adds a made playlist to the users library of playlists
	void addNewPlaylist(const std::string& nameOfPlaylist); // adds a new empty playlist to the user's library, the user can then add songs one by one to it

};
