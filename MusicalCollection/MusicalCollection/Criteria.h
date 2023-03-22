#pragma once
#include "Song.h"
#include <queue>
#include<set>

class Criteria
{
private:
	int rating = 0;
	int yearOfRelease = 0;
	int signBeforeTheYear = -1; // 0-before 1-current 2-after
	Genre genre = Genre::None;
	bool onlyFavorites = false;

public:
	Criteria(int rating, int yearOfRelease, int signBeforeRelease, std::string genre, bool onlyFavorites);

	int checkType() const; // 1-rating 2-genre 3-yearOfRelease 4-onlyFavorites

	//get methods
	int getRating() const;
	Genre getGenre() const;
	int getYearOfRelease() const;
	int getSignBeforeTheYear() const;
	bool getOnlyFavorites() const;
};
