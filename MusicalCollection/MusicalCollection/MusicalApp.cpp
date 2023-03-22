#include "MusicalApp.h"

void MusicalApp::addSongToAllSongsDeserialize(const std::string& title, const std::string& artist, const std::string& album, short releaseYear, const std::string& genre, std::map<std::string, float> ratings) {
	if (getSongBytitle(title) != nullptr)
	{
		std::cout << "You are trying to add a song that already exists.";
	}

	Song* song = new Song(title, artist, album, releaseYear, Song::stringTogenre(genre));
	song->addRatingsDeserialize(ratings);
	allSongs[title] = song;
}

void MusicalApp::serialize()
{
	std::ofstream songsFile;
	std::ofstream usersFile;
	songsFile.open("Songs.txt",std::ios::out);
	if (songsFile.is_open()) {
		for(auto song : allSongs)
		{
			(*song.second).serializeSong(songsFile);
		}

	}
	songsFile.close();

	usersFile.open("Users.txt", std::ios::out);
	if (usersFile.is_open()) {
		for (std::pair<std::string, User> user : allUsers) {
			user.second.serializeUser(usersFile);
		}
	}
	usersFile.close();

}

void MusicalApp::deserialize()
{
	std::ifstream songsFile;
	std::ifstream usersFile;
	std::string title;
	std::string artist;
	std::string album;
	short releaseYear;
	std::string genre;
	std::string ratingsInput;
	std::map<std::string, float> ratings;

	std::string username;
	std::string pass;
	std::string fullName;
	std::string bDay;
	std::string genresInput;
	std::vector<Genre> favoriteGenres;

	std::string nameOfPlaylist;
	std::string titleOfSong;
	
	std::string nextInput;

	songsFile.open("Songs.txt", std::ios::in);
	if (songsFile.is_open()) {
		while (!songsFile.eof()) {

			getline(songsFile, title);
			if (title == "") {
				continue;
			}

			getline(songsFile, artist);
			getline(songsFile, album);
			songsFile >> releaseYear;
			songsFile.ignore();
			getline(songsFile, genre);
			getline(songsFile, ratingsInput);
			
			size_t pos = 0;
			while ((pos = ratingsInput.find(' ')) != std::string::npos) {

				std::string username = ratingsInput.substr(0, pos);
				ratingsInput.erase(0, pos + 1);

				pos = ratingsInput.find(' ');
				float numberForRating = std::stof(ratingsInput.substr(0, pos)); //std::stof makes a string into float
				ratingsInput.erase(0, pos + 1);

				ratings[username] = numberForRating;
			}
			if (ratingsInput != "" && ratingsInput != " ") {
				std::cout << "Error during deserializing ratings of a song." << std::endl;
				return;
			}
			
			addSongToAllSongsDeserialize(title, artist,album, releaseYear, genre,ratings);
		}
	}

	songsFile.close();
	usersFile.open("Users.txt", std::ios::in);
	if (usersFile.is_open()) {
		while (!usersFile.eof()) {

			getline(usersFile, username);
			if (username == "") {
				continue;
			}

			getline(usersFile, pass);
			getline(usersFile, fullName);
			getline(usersFile, bDay);

			getline(usersFile, genresInput);
			if (genresInput != "") { // the user may not have favorite genres
				size_t pos = 0;
				while ((pos = genresInput.find(' ')) != std::string::npos) {
					favoriteGenres.push_back(Song::stringTogenre(genresInput.substr(0, pos)));
					genresInput.erase(0, pos + 1);
				}
				if (genresInput != "") {
					favoriteGenres.push_back(Song::stringTogenre(genresInput));
				}
			}

			User userToAdd = User(username, pass, fullName, bDay, favoriteGenres); //saving in variable to add playlists to it

			favoriteGenres.clear();
			
			getline(usersFile, nextInput);
			if (nextInput != "[")
			{
				std::cout << "Error in deserializing playlists." << std::endl;
				return;
			}

			while (nextInput != "]") {
				getline(usersFile, nextInput);
				if (nextInput == "]") {
					continue;
				}

				std::forward_list<const Song*> songsInPlaylist;
				nameOfPlaylist = nextInput;

				getline(usersFile, nextInput);
				if (nextInput != "{") {
					std::cout << "Error in deserializing playlists." << std::endl;
					return;
				}

				while (nextInput != "}") {
					getline(usersFile, nextInput);
					if (nextInput == "}") {
						continue;
					}
					songsInPlaylist.push_front(getSongBytitle(nextInput));
				}
				userToAdd.addPlaylist(Playlist(nameOfPlaylist, songsInPlaylist));
			}
			
			allUsers[username] = userToAdd;
		}
	}
	usersFile.close();
}

const Song* MusicalApp::getSongBytitle(const std::string& title){

	std::map<std::string, Song*>::iterator itr = allSongs.find(title);
	if (itr != allSongs.end()) {
		return (*itr).second;
	}
	return nullptr;

}

void MusicalApp::addUserToAllUsers(const std::string& username, const std::string& pass, const std::string& fullName, const std::string& bDay, std::vector<Genre> favGenres, std::vector<Playlist> playlists)
{
	User newUser(username,pass, fullName, bDay, favGenres);
	for (size_t i = 0; i < playlists.size(); i++)
	{
		newUser.addPlaylist(playlists[i]);
	}
	allUsers[username] = newUser;
}

bool MusicalApp::logIn(const std::string&  username,const std::string& pass)
{
	std::map<std::string, User>::iterator itr = allUsers.find(username);
	if (itr != allUsers.end())
	{
		User user = (*itr).second; //dereferencing the iterator
		if (user.getPass() == pass) {
			std::cout << "Welcome, " << username << std::endl;
			currLoggedUser = username;
			return true;
		}
	}

	std::cout << "Invalid username or password." << std::endl;
	return false;
}

void MusicalApp::SingUp()
{
	std::string username;
	std::string pass;
	std::string fullName;
	std::string bDay;
	std::vector<Genre> favGenres;

	bool flag = true;

	while (username.empty()) {
		std::cout << "Enter an username: ";
		getline(std::cin, username);
		std::map<std::string, User>::iterator itr = allUsers.find(username);
		if (itr != allUsers.end()) {
			std::cout << "This username is already taken."<<std::endl;
			username = "";
			continue;
		}
	}

	while (pass.empty()) {
		std::cout << "Enter a password with at least 8 symbols: ";
		getline(std::cin, pass);
		if (pass.size() < 8) {
			std::cout << "This password is too short. " << std::endl;
			pass = "";
		}
	}

	while (fullName.empty()) {
		std::cout << "Enter your full name: ";
		getline(std::cin, fullName);
	}
	while (bDay.empty()) {
		std::cout << "Enter your birthday (ddmmyyyy): "; // bday0 - 0,1,2,3 bday2 - 0,1 bday4 - 1,2 bday5 - 9,0 -> 29092001
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

	while (flag) {
		std::string answerAddingGenre;
		bool genreAlreadyExists = false;
		std::cout << "Do you want to add to your favourite genres? (y/n)";
		getline(std::cin, answerAddingGenre);
		if (answerAddingGenre == "y" || answerAddingGenre == "Y") {
			std::string genre;
			std::cout << "Please enter one genre - Pop/Rock/RnB/Rap" << std::endl;
			getline(std::cin, genre);
			for (size_t i = 0; i < favGenres.size(); i++)
			{
				if (Song::genreToString(favGenres[i]) == genre) {
					std::cout << "This genre is already in your favorites. " << std::endl;
					genreAlreadyExists = true;
					break;
				}
			}
			if (genreAlreadyExists) {
				continue;
			}
			Genre convertedGenre = Song::stringTogenre(genre);
			if (convertedGenre == Genre::None) {
				std::cout << "This is not a possible genre." << std::endl;
				continue;
			}
			favGenres.push_back(convertedGenre);
		}
		else if (answerAddingGenre == "n" || answerAddingGenre == "N") {
			flag = false;
			continue;
		}
		else
			std::cout << "Invalid answer." << std::endl;
	}
	User newUser(username, pass, fullName, bDay, favGenres);
	allUsers[username] = newUser;
	currLoggedUser = username;
}


void MusicalApp::addSongToAllSongs()
{
	if (currLoggedUser == "") {
		std::cout << "You are not logged in." << std::endl;
		return;
	}
	std::string title;
	std::string artist;
	std::string album;
	short releaseYear = 0;
	std::string genre;

	while (title.empty()) {
		std::cout << "Enter the name for the song: ";
		getline(std::cin, title);
	}

	if (getSongBytitle(title) != nullptr)
	{
		std::cout << "You are trying to add a song that already exists." << std::endl;
		return;
	}

	while (artist.empty()) {
		std::cout << "Enter the artist's name: ";
		getline(std::cin, artist);
	}
	while (album.empty()) {
		std::cout << "Enter the album the song is in: ";
		getline(std::cin, album);
	}
	while (releaseYear == 0) {
		std::cout << "Enter the year the song was released in: ";
		std::cin >> releaseYear;
		if (releaseYear < 1000 || releaseYear>2023) {
			releaseYear = 0;
			std::cout << "Invalid year."<<std::endl;
		}
	}
	while (genre.empty()) {
		std::cin.ignore();
		std::cout << "Enter the genre of the song (Pop/Rock/Rap/RnB): ";
		getline(std::cin, genre);
		if (genre != "Pop" && genre != "Rap" && genre != "Rock" && genre != "RnB") {
			std::cout << "Invalid genre." << std::endl;
			genre = "";
		}
	}

	Song* song = new Song(title, artist,album, releaseYear, Song::stringTogenre(genre));
	allSongs[title] = song;
	std::cout << title << " has been successfully added to the library." << std::endl;
}

void MusicalApp::addSongToPlaylist()
{
	if (currLoggedUser == "") {
		std::cout << "You are not logged in." << std::endl;
		return;
	}
	std::string titleOfSong = "";
	std::string nameOfPlaylist = "";
	
	while (titleOfSong == "") {
		std::cout << "Enter the title of the song you want to add to a playlist: ";
		getline(std::cin, titleOfSong);
	}

	std::map<std::string, Song*>::iterator itr = allSongs.find(titleOfSong);
	if (itr != allSongs.end()) {
		while (nameOfPlaylist == "") {
			std::cout << "Enter the name of the playlist you want to add this song to: ";
			getline(std::cin, nameOfPlaylist);
		}
		(*allUsers.find(currLoggedUser)).second.addSongToPlaylist(getSongBytitle(titleOfSong), nameOfPlaylist);
		return;
	}
	std::cout << "This song is not in the library." << std::endl;
	
}

void MusicalApp::changeData()
{
	int inputAnswer = 0;
	std::map<std::string, User>::iterator itr = allUsers.find(currLoggedUser);
	if (currLoggedUser == "") {
		std::cout << "You are not logged in." << std::endl;
		return;
	}
	std::cout << "What do you want to change:\n 1. name\n 2. Birthday\n 3.password\n 4. add to your favorite genres\n 5. remove from your favorite genres\n 6. nothing" << std::endl;
	std::cin >> inputAnswer;
	std::cin.ignore();
	switch (inputAnswer)
	{
	case 1:
		(*itr).second.changeName();
		break;
	case 2:
		(*itr).second.changeBday();
		break;
	case 3:
		(*itr).second.changePass();
		break;
	case 4:
		(*itr).second.addFavGenre();
		break;
	case 5:
		(*itr).second.removeFavGenre();
		break;
	case 6:
		break;
	default: std::cout << "This is not a possible option.";
		break;
	}
}

void MusicalApp::loadPlaylist() 
{
	if (currLoggedUser == "") {
		std::cout << "You are not logged in." << std::endl;
		return;
	}

	std::string nameOfPlaylist = "";

	while (nameOfPlaylist.empty()) {
		std::cout << "Enter the name of the playlist you want to load: ";
		std::cin >> nameOfPlaylist;
	}
	if ((*allUsers.find(currLoggedUser)).second.loadPlaylist(nameOfPlaylist)) {
		std::cout << "Loaded playlist: " << nameOfPlaylist << std::endl;
	}
	else {
		std::cout << "There is no such playlist in your library." << std::endl;
		return;
	}
}

void MusicalApp::createPlaylist()
{
	if (currLoggedUser == "") {
		std::cout << "You are not logged in." << std::endl;
		return;
	}

	std::string nameForPlaylist;
	while (nameForPlaylist == "") {
		std::cout << "Enter a name for the new playlist: ";
		getline(std::cin, nameForPlaylist);
	}
	(*allUsers.find(currLoggedUser)).second.addNewPlaylist(nameForPlaylist);
}

void MusicalApp::rateSong(const std::string& titleOfSong)
{
	float rating = 0;
	if (currLoggedUser == "") {
		std::cout << "You are not logged in." << std::endl;
		return;
	}

	std::map<std::string, Song*>::iterator itr = allSongs.find(titleOfSong);
	if (itr != allSongs.end()) {
		while (rating == 0) {
			std::cout << "Enter a rating for this song 1-5: ";
			std::cin >> rating;
			if (rating < 1 || rating > 5) {
				std::cout << "Invalid rating." << std::endl;
				rating = 0;
			}
		}
		(*itr).second->rateThisSong(currLoggedUser, rating);
		return;
	}
	std::cout << "Song was not found." << std::endl;
}

void MusicalApp::generatePlaylist()
{
	std::string input;
	size_t sizeOfPlaylist;
	std::string nameOfPlaylist;
	std::forward_list<const Song*> songsForPlaylist;
	std::map<std::string,User>::iterator currUser = allUsers.find(currLoggedUser);
	std::map<std::string, Playlist> playlistsOfUser = (*currUser).second.getPlaylists();

	
	while (nameOfPlaylist == "") {
		std::cout << "Enter a name for the playlist: ";
		getline(std::cin, nameOfPlaylist);
		if (playlistsOfUser.find(nameOfPlaylist) != playlistsOfUser.end()) {
			std::cout << "There is an existing playlist with that name." << std::endl;
			nameOfPlaylist = "";
		}

	}

	std::cout << "How many songs do you want the playlist to have: ";
	std::cin >> sizeOfPlaylist;

	std::queue<Criteria> criterias = getCriterias();

	while (input == "") {
		std::cin.ignore();
		std::cout << "Do you want these criterias to be combined with AND or OR: ";
		getline(std::cin,input);
		if (input != "AND" && input != "and" && input != "OR" && input != "or") {
			std::cout << "Invalid answer." << std::endl;
			input = "";
		}
	}
	
	if (input == "AND" || input == "and") {
		std::map<std::string, Song*> filteredSongsWithAnd;
		filteredSongsWithAnd = FilterSongsWithAnd(criterias);
		size_t counterOfSongs = 0;
		for(auto song: filteredSongsWithAnd)
		{
			if (counterOfSongs == sizeOfPlaylist) {
				break;
			}
			songsForPlaylist.push_front(song.second);
			counterOfSongs++;

		}

	}
	else if (input == "OR" || input == "or") {
		std::set<const Song*> filteredSongsWithOr;
		filteredSongsWithOr = FilterSongsWithOr(criterias);
		size_t counterOfSongs = 0;
		for (auto song : filteredSongsWithOr) {

			if (counterOfSongs == sizeOfPlaylist) {
				break;
			}
			songsForPlaylist.push_front(song);
			counterOfSongs++;
		}
	}

	Playlist newPlaylist(nameOfPlaylist, songsForPlaylist);
	(*currUser).second.addPlaylist(newPlaylist);
	std::cout << "You've successfully created " << nameOfPlaylist;
	
}

std::queue<Criteria> MusicalApp::getCriterias()
{
	std::queue<Criteria> criterias;
	int numberOfCriteria = 0;
	while (numberOfCriteria != 5)
	{
		std::cout << "Number of criteria:\n 1. The songs to be above a certain rating\n 2. The songs to be from a specific genre\n";
			std::cout<<" 3. The releasing of the song to be before/during/after a certain year\n 4. The songs to be form your favrite genres\n 5. Exit\n";
			std::cout << "Note: The criterias will be with priority of the order they were entered.\n";
		std::cin >> numberOfCriteria;
		switch (numberOfCriteria)
		{
		case 1:
		{
			int rating = 0;
			while (rating < 1 || rating > 5)
			{
				std::cout << "Enter rating: ";
				std::cin >> rating;
			}
			criterias.push(Criteria(rating, 0, -1, "", false));
		}
		break;
		case 2:
		{
			std::string genre = "";
			while (genre != "Rock" && genre != "Pop" && genre != "Rap" && genre != "RnB")
			{
				std::cout << "Enter a genre(Rock/Pop/Rap/RnB):";
				std::cin >> genre;
			}
			criterias.push(Criteria(0, 0, -1, genre, false));
		}
		break;
		case 3:
		{
			int yearOfRelease = 0;
			int signBeforeYear = -1;
			while (yearOfRelease < 1000 || yearOfRelease > 2023 || signBeforeYear < 0 || signBeforeYear > 2)
			{
				std::cout << "Enter year of release";
				std::cin >> yearOfRelease;
				std::cout << "Songs need to be released...Enter a number: \n 0. Before the year you entered\n ";
				std::cout << "1. During the year you entered\n 2. After the year you entered.\n";
				std::cin >> signBeforeYear;
			}
			criterias.push(Criteria(0, yearOfRelease, signBeforeYear, "", false));
		}
		break;
		case 4:
		{
				criterias.push(Criteria(0, 0, -1, "", true));
		}
		break;
		case 5:
		{
			break;
		}
		default:
			std::cout << "No such criteria" << std::endl;
			break;
		}
	}
	return criterias;
}

std::map<std::string, Song*> MusicalApp::FilterSongsWithAnd(std::queue<Criteria> criterias)
{
	std::map<std::string, Song*> filteredSongs = allSongs;
	while (!criterias.empty())
	{
		Criteria criteria = criterias.front();
		criterias.pop();
		int type = criteria.checkType();

		switch (type)
		{
		case 1:
		{
			for (auto song = filteredSongs.begin(); song != filteredSongs.end();) // using iterators because of erase()
			{
				if ((*song).second->getAverageRating() < criteria.getRating()){
					song = filteredSongs.erase(song); // if the rating is less than the desired erase erases the song and returns an iterator for the next element
				}
				else{
					song++; //the element remains if the condition is met
				}
			}
		}
		break;
		case 2:
		{
			for (auto song = filteredSongs.begin(); song != filteredSongs.end();){
				if ((*song).second->getGenre() != criteria.getGenre()){
					song = filteredSongs.erase(song);
				}
				else{
					song++;
				}
			}
		}
		break;
		case 3:
		{
			for (auto song = filteredSongs.begin(); song != filteredSongs.end();)
			{
				int signBeforeReleaseYear = criteria.getSignBeforeTheYear();
				if (signBeforeReleaseYear == 0){
					if ((*song).second->getReleaseYear() > criteria.getYearOfRelease()){
						song = filteredSongs.erase(song);
					}
				}
				else if (signBeforeReleaseYear == 1){
					if ((*song).second->getReleaseYear() != criteria.getYearOfRelease()){
						song = filteredSongs.erase(song);
					}
				}
				else if (signBeforeReleaseYear == 2){
					if ((*song).second->getReleaseYear() < criteria.getYearOfRelease()){
						song = filteredSongs.erase(song);
					}
				}
				else{
					song++;
				}
			}
		}
		break;
		case 4:
		{
			User currUser = (*allUsers.find(currLoggedUser)).second;
			std::vector<Genre> favGenres = currUser.getFavGenres();
			for (auto song = filteredSongs.begin(); song != filteredSongs.end();) {
				bool isFromFavorites = false;
				for (size_t i = 0; i < favGenres.size(); i++)
				{
					if ((*song).second->getGenre() == favGenres[i]) {
						isFromFavorites = true;
						break;
					}
				}
				if (isFromFavorites) {
					song++;
				}
				else {
					song = filteredSongs.erase(song);
				}
				
			}

		}
		break;
		default:
		{
			std::cout << "Unhandled criteria!" << std::endl;
		}
		break;
		}
	}

	return filteredSongs;
}

std::set<const Song*> MusicalApp::FilterSongsWithOr(std::queue<Criteria> criterias)
{
	std::set<const Song*> finalSongs; //adding the filtered songs here without the repetitions

	while (!criterias.empty()){
		Criteria criteria = criterias.front();
		criterias.pop();
		int type = criteria.checkType();

		switch (type){
		case 1:
		{
			for (auto song : allSongs){
				if (song.second->getAverageRating() >= criteria.getRating()){
					finalSongs.insert(song.second);
				}
			}
		}
		break;
		case 2:
		{
			for (auto song : allSongs){
				if (song.second->getGenre() == criteria.getGenre()){
					finalSongs.insert(song.second);
				}
			}
		}
		break;
		case 3:
		{
			for (auto song : allSongs){
				int signBeforeReleaseYear = criteria.getSignBeforeTheYear();
				if (signBeforeReleaseYear == 0){
					if (song.second->getReleaseYear() < criteria.getYearOfRelease()){
						finalSongs.insert(song.second);
					}
				}
				else if (signBeforeReleaseYear == 1){
					if (song.second->getReleaseYear() == criteria.getYearOfRelease()){
						finalSongs.insert(song.second);
					}
				}
				else if (signBeforeReleaseYear == 2){
					if (song.second->getReleaseYear() > criteria.getYearOfRelease()){
						finalSongs.insert(song.second);
					}
				}
			}
		}
		break;
		case 4:
		{
			User currUser = (*allUsers.find(currLoggedUser)).second;
			std::vector<Genre> favGenres = currUser.getFavGenres();

			for (auto song : allSongs){
				bool isFromFavorites = false;
				for (size_t i = 0; i < favGenres.size(); i++)
				{
					if (song.second->getGenre() == favGenres[i]) {
						isFromFavorites = true;
						break;
					}
				}

				if (isFromFavorites) {
					finalSongs.insert(song.second);
				}

			}
		}
		break;
		default:
		{
			std::cout << "Unhandled criteria!" << std::endl;
		}
		break;
		}
	}

	return finalSongs;
}

void MusicalApp::SongsInfoFromCurrPlaylist()
{
	if (currLoggedUser == "") {
		std::cout << "You are not logged in." << std::endl;
		return;
	}
	std::map<std::string, User>::iterator itr = allUsers.find(currLoggedUser);
	if (itr != allUsers.end()) {
		(*itr).second.currPlaylistSongsInfo();
	}
}

void MusicalApp::run()
{
	bool flag = true;
	while (flag) {
		int input;
		std::string nameOfSongToRate;
		std::cout << std::endl;
		std::cout << "What do you want to do? Enter a number: " << std::endl;
		std::cout << "1. Add song to one of your playlists." << std::endl;
		std::cout << "2. Add song to the library of songs." << std::endl;
		std::cout << "3. Change name/password/birthday/add or remove a favorite genre. " << std::endl;
		std::cout << "4. Create playlist." << std::endl;
		std::cout << "5. Load playlist." << std::endl;
		std::cout << "6. Rate a song." << std::endl;
		std::cout << "7. Show info for the songs from the currently loaded playlist. " << std::endl;
		std::cout << "8. Generate playlist. " << std::endl;
		std::cout << "9. Exit." << std::endl;
		std::cin >> input;
		std::cin.ignore();
		switch (input)
		{
		case 1:
			addSongToPlaylist();
			break;
		case 2:
			addSongToAllSongs();
			break;
		case 3:
			changeData();
			break;
		case 4:
			createPlaylist();
			break;
		case 5:
			loadPlaylist();
			break;
		case 6:
			std::cout << "Enter the name of the song: ";
			getline(std::cin, nameOfSongToRate);
			rateSong(nameOfSongToRate);
			break;
		case 7:
			SongsInfoFromCurrPlaylist();
			break;
		case 8:
			generatePlaylist();
			break;
		case 9:
			flag = false;
			break;
		default:
			std::cout << "Invalid input." << std::endl;
			break;
		}
	}

}


