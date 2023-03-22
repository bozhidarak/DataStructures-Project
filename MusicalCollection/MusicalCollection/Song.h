#pragma once
#include<iostream>
#include<string>
#include<map>
#include<fstream>

enum class Genre{
	Pop,
	Rock,
	RnB,
	Rap,
	None
};
class Song
{
private:

	std::string title; // used as ID - it is unique, can't have 2 songs with the same title
	std::string artist;
	std::string album;
	short releaseYear;
	Genre genre;
	std::map<std::string, float> ratings;// map - username,rating -> logN complexity for access and insertion using red-black tree

public:
	Song() = delete; // a song should not be created by default constructor, only with constructor with parameters
	Song(const std::string& title, const std::string& artist, const std::string& album, short releaseYear, Genre genre);

	void serializeSong(std::ofstream& songFile);

	//static so I can use them without an object from class Song
	static std::string genreToString(Genre genre);
	static Genre stringTogenre(const std::string& genre); //& - using less space by not doing a copy

	//get methods
	std::string getTitle() const;
	std::string getArtist() const;
	std::string getAlbum() const;
	short getReleaseYear() const;
	float getAverageRating() const; //calculates the average rating of a song based on the users that voted
	Genre getGenre() const;

	//ratings
	void rateThisSong(const std::string& userName, float rating); //every user can vote only once
	void addRatingsDeserialize(std::map<std::string, float> ratings);



};