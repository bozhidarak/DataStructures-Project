#include "User.h"

User::User()
{
}

User::User(const std::string& username, const std::string& pass, const std::string& fullName, const std::string& bDay, std::vector<Genre> favGenres)
{
	this->username = username;
	this->pass = pass;
	this->fullName = fullName;
	this->bDay = bDay;
	this->favoriteGenres = favGenres;
	this->currPlaylist = "";
}


void User::serializeUser(std::ofstream& usersFile)
{
	usersFile << username << '\n';
	usersFile << pass << '\n';
	usersFile << fullName << '\n';
	usersFile << bDay << '\n';
	if (favoriteGenres.empty()) {
		usersFile << '\n';
	}
	else {
		for (size_t i = 0; i < favoriteGenres.size(); i++)
		{
			usersFile << Song::genreToString(favoriteGenres[i]) << ' ';
		}
		usersFile << '\n';
	}
		usersFile << '[' << '\n';
		for (auto playlist : playlists)
		{
			playlist.second.serializePlaylist(usersFile);
		}
		usersFile << ']' << '\n';
}

void User::changeName()
{
	std::string newName = "";
	while (newName == "") {
		std::cout << "Enter a new full name: ";
		getline(std::cin, newName);
	}
	this->fullName = newName;
	std::cout << "Your full name has been changed to: " << newName << std::endl;
}

void User::changeBday()
{
	bDay = "";
	while (bDay == "") {
		std::cout << "Enter a new birthday (ddmmyyyy): "; // bday0 - 0,1,2,3 bday2 - 0,1 bday4 - 1,2 bday5 - 9,0 -> 29092001
		getline(std::cin, bDay);
		if ((bDay[0] != '0' && bDay[0] != '1' && bDay[0] != '2' && bDay[0] != '3')) {
			if (bDay[2] != '0' && bDay[2] != '1') {
				if (bDay[4] != '1' && bDay[4] != '2') {
					if (bDay[5] != '9' && bDay[5] != '0') {

						std::cout << "Invalid date." << std::endl;
						bDay = "";

					}
				}
			}
		}
	}
	std::cout << "Your birthday has been changed to: " << bDay<< std::endl;

}

void User::changePass()
{
	std::string newPass = "";
	while (newPass == "") {
		std::cout << "Enter a new password with at least 8 symbols: ";
		getline(std::cin, newPass);
		if (newPass.size() < 8) {
			std::cout << "This password is too short. " << std::endl;
			newPass = "";
		}
	}
	this->pass = newPass;
	std::cout << "Your password has been changed. "<<std::endl;
}

void User::addFavGenre()
{
	std::string genreToAdd = "";
	std::cin.ignore();
	while (genreToAdd == "")
	{
		std::cout << "Which genre would you like to add to your favorites? (Pop/Rock/RnB/Rap)" << std::endl;
		getline(std::cin, genreToAdd);
		if (genreToAdd != "Rock" && genreToAdd != "Pop" && genreToAdd != "RnB" && genreToAdd != "Rap") {
			genreToAdd = "";
		}
	}
	Genre genre = Song::stringTogenre(genreToAdd);
	for (size_t i = 0; i < favoriteGenres.size(); i++)
	{
		if (genre == favoriteGenres[i]) {
			std::cout << "This genre is alreary in your favorites." << std::endl;
			return;
		}
	}

	favoriteGenres.push_back(genre);
	std::cout << "You've added " << genreToAdd << " to your favorites." << std::endl;
}

void User::removeFavGenre()
{
	std::string genreToRemove = "";
	std::cin.ignore();
	while (genreToRemove == "") {
		std::cout << "Which genre would you like to remove from your favorites? (Pop/Rock/RnB/Rap)" << std::endl;
		getline(std::cin, genreToRemove);
		if (genreToRemove != "Rock" && genreToRemove != "Pop"&& genreToRemove != "RnB" && genreToRemove != "Rap") {
			genreToRemove = "";
		}
	}
	Genre genre = Song::stringTogenre(genreToRemove);
	for (std::vector<Genre>::iterator itr = favoriteGenres.begin(); itr != favoriteGenres.end(); itr++)
	{
		if (genre == *itr) {
			favoriteGenres.erase(itr);
			std::cout<<"You've removed " << genreToRemove<< " from your favorites." << std::endl;
			return;
		}
	}
	std::cout << "There is no such genre in your favorites." << std::endl;
}


void User::addSongToPlaylist(const Song* toAdd,const std::string& nameOfPlaylist) {
	
	if (playlists.empty()) {
		std::cout << "You have no playlists.";
		return;
	}
	std::map<std::string, Playlist>::iterator itrPlaylists = playlists.find(nameOfPlaylist);
	if(itrPlaylists != playlists.end()){
		(*itrPlaylists).second.addSongToThisPlaylist(toAdd);
		std::cout << "You've successfully added " << toAdd->getTitle() << " to " << nameOfPlaylist << std::endl;
		return;
	}

	std::cout << "There is no such playlist in your library.";
}

std::vector<Genre> User::getFavGenres() const
{
	return favoriteGenres;
}

void User::addSongToCurrPlayList(const Song* songToAdd)
{
	std::map<std::string, Playlist>::iterator itrPlaylists = playlists.find(currPlaylist);
	if (itrPlaylists != playlists.end()) {
		(*itrPlaylists).second.addSongToThisPlaylist(songToAdd);
	}
}

std::map<std::string,Playlist> User::getPlaylists() const
{
	return playlists;
}

std::string User::getUserName() const
{
	return username;
}

std::string User::getPass() const
{
	return pass;
}

bool User::loadPlaylist(const std::string& nameOfPlaylist)
{
	std::map<std::string, Playlist>::iterator itrPlaylists = playlists.find(nameOfPlaylist);
	if (itrPlaylists != playlists.end()) {
		currPlaylist = (*itrPlaylists).second.getName();
		return true;
	}
	return false;
}

void User::currPlaylistSongsInfo()
{
	if (currPlaylist == "") {
		std::cout << "There isn't a loaded playlist." << std::endl;
		return;
	}
	std::map<std::string, Playlist>::iterator itrPlaylists = playlists.find(currPlaylist);
	if (itrPlaylists != playlists.end()) {
		(*itrPlaylists).second.printSongs();
	}
}

void User::addPlaylist(const Playlist& playlist)
{
	playlists[playlist.getName()] = playlist;
}

void User::addNewPlaylist(const std::string& nameOfPlaylist)
{
	if (playlists.find(nameOfPlaylist) != playlists.end()) {
		std::cout << "A playlist with that name already exists." << std::endl;
		return;
	}

	Playlist newPlaylist(nameOfPlaylist);

	playlists[nameOfPlaylist] = newPlaylist;
	std::cout << "You've successfully created " << nameOfPlaylist << std::endl;
}
