#pragma once
#include "User.h"
#include "Criteria.h"
#include<fstream>
class MusicalApp
{
private:

	std::map<std::string, Song*> allSongs; // map - title,song -> logN complexity for access and insertion using red-black tree
	std::map<std::string,User> allUsers; // map - username,user
	std::string currLoggedUser;

	//this method is only accessed during deserialization
	void addSongToAllSongsDeserialize(const std::string& title,const std::string& artist,const std::string& album, short releaseYear, const std::string& genre, std::map<std::string, float> ratings);

public:

	void serialize();
	void deserialize();

	const Song* getSongBytitle(const std::string& title);

	void addUserToAllUsers(const std::string& username, const std::string& pass,const std::string& fullName,const std::string& bDay, std::vector<Genre> favGenres, std::vector<Playlist> playlists);

	//for User 
	bool logIn(const std::string& username, const std::string& pass);
	void SingUp();
	void addSongToAllSongs(); //if a song is not in allSongs a user can add it with all it's info
	void addSongToPlaylist(); //the logged user can add a song to an existing playlist
	void changeData(); //changing full name, birthday, pass, adding or removing favorite genre
	void loadPlaylist();//finds the playlist by name and makes it currPlaylist
	void createPlaylist(); //allows the user to create an empty playlist with only a name
	void rateSong(const std::string& titleOfSong);//a user can rate a song only once

	//methods for generating playlist
	void generatePlaylist();
	std::queue<Criteria> getCriterias();
	std::map<std::string, Song*> FilterSongsWithAnd(std::queue<Criteria> criterias);
	std::set<const Song*> FilterSongsWithOr(std::queue<Criteria> criterias); //using set so each song apears once

	void SongsInfoFromCurrPlaylist(); // prints information about the songs in the currently loaded playlist

	void run(); // contains the logic for the user's interaction with the program

};

