#include <iostream>
#include"MusicalApp.h"

int main()
{
	MusicalApp app;
	int input;
	std::string username;
	std::string pass;

	app.deserialize();

	std::cout << "Hi! Please enter a number:\n 1. Login\n 2. Signup" << std::endl;
	std::cin >> input;
	std::cin.ignore();
	if (input == 1) {
		std::cout << "Enter username: ";
		getline(std::cin, username);
		std::cout << "Enter password: ";
		getline(std::cin, pass);;
		if (app.logIn(username, pass)) {
			app.run();
		}
	}
	else if (input == 2) {
		app.SingUp();
		app.run();
	}
	else {
		std::cout << "Invalid input." << std::endl;
	}

	app.serialize();
} 