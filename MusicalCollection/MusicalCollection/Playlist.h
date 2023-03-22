#pragma once
#include<iostream>
#include<forward_list>
#include "Song.h"
class Playlist
{
private:
	std::string name;
	std::forward_list<const Song*> songs; //O(1) compexity for inserting into the list
public:
	Playlist();
	Playlist(const std::string& name);
	Playlist(const std::string& name, std::forward_list<const Song*> songs);

	void serializePlaylist(std::ofstream& usersFile);

	void addSongToThisPlaylist( const Song* song);
	void printSongs();

	std::string getName() const;
	
};

