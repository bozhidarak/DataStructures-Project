#include "Criteria.h"



Criteria::Criteria(int rating, int yearOfRelease, int signBeforeRelease, std::string genre, bool onlyFavorites){
	this->rating = rating;
	this->yearOfRelease = yearOfRelease;
	this->signBeforeTheYear = signBeforeRelease;
	this->genre = Song::stringTogenre(genre);
	this->onlyFavorites = onlyFavorites;
}

int Criteria::checkType() const
{
	if (rating != 0){
		return 1;
	}
	else if (genre != Genre::None){
		return 2;
	}
	else if (yearOfRelease >= 1000 && signBeforeTheYear != -1){
		return 3;
	}
	else if (onlyFavorites == true){
		return 4;
	}
}

int Criteria::getRating() const
{
	return rating;
}

Genre Criteria::getGenre() const
{
	return genre;
}

int Criteria::getYearOfRelease() const
{
	return yearOfRelease;
}

int Criteria::getSignBeforeTheYear() const
{
	return signBeforeTheYear;
}

bool Criteria::getOnlyFavorites() const
{
	return onlyFavorites;
}