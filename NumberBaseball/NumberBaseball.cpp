// NumberBaseball.cpp : Defines the entry point for the console application.
//

// #include "stdafx.h"
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <windows.h>
#include <fstream>
#include <map>

#include "json.hpp"
#include "leaderboard.h"
#include "guess.h"

using json = nlohmann::json;
using namespace std;

void SetStdinEcho(bool enable = true)
{
#ifdef WIN32
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);

	if (!enable)
		mode &= ~ENABLE_ECHO_INPUT;
	else
		mode |= ENABLE_ECHO_INPUT;

	SetConsoleMode(hStdin, mode);

#else
	struct termios tty;
	tcgetattr(STDIN_FILENO, &tty);
	if (!enable)
		tty.c_lflag &= ~ECHO;
	else
		tty.c_lflag |= ECHO;

	(void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

int main()
{
	cout << "This program is a baseball number game which you can compete with AI or with a friend" << endl << endl;	
	cout << "Rules:" << endl << "Basically, it is a number guessing game." << endl;
	cout << "You write down 3 digit number without any duplicate number." << endl;
	cout << "And computer will also generate a random 3 digit number" << endl;
	cout << "Each turn, you and computer have to guess the each other's number." << endl;
	cout << "If one of them gets the other's number then the game is over." << endl;
	cout << "If the guess is not fully correct, then there are some rules." << endl;
	cout << "If the location and the digit is correct then that is \'Strike\'." << endl;
	cout << "If only the location is correct then that is \'Ball\'."  << endl;
	cout << "For example, the answer is 123 and the guess is 328, then it is 1 Strike 1 Ball." << endl;
	cout << "Since 3 is there but the location is wrong (ball) and 2 is at the same location (strike)." << endl;
	cout << "To play with a computer, type 1. To Play 2 player, type 2: ";

	// start 
	string any1;
	cin >> any1;

	// vs CPU
	if (any1 == "1")
	{
		string Answer, Guess;				// computer's answer and guess
		string UserGuess;					// user's guess
		string UserName;
		int trycount = 0;					// for report 
		int strike, ball;
		int level;
		bool userWin = false, computerWin = false;

		// generation of the cpu number

		Answer = numberGenerator();
		vector<string> guesses;
		createGuesses(&guesses);


		cout << endl << endl << "This is AI Mode. Good Luck!" << endl;
		cout << "Create a three digit number that meets the criteria." << endl;
		cout << "input level choice: 1-> beginner, 2-> medium, 3-> hard : ";
		
	
		while (1)
		{
			if (cin >> level && level > 0 && level < 4)
			{
				break;
			}
			else
			{
				cout << "Error" << endl << "input level choice: 1-> beginner, 2-> medium, 3-> hard : ";
				cin.clear();
				while (cin.get() != '\n');
			}
		}

		
		cout << endl;
		switch (level)
		{
		case 1: cout << "Beginner mode" << endl;
			break;
		case 2: cout << "Medium mode" << endl;
			break;
		case 3: cout << "Hard mode" << endl;
			break;
		}

		printleaderboard(level);

		cout << "Input your username: ";
		cin.ignore();
		cin.clear();
		getline(cin, UserName);

		while (!(userWin || computerWin))
		{
			cout << endl << endl << "Turn " << ++trycount << endl;

			// User Guess
			cout << UserName << " Guesses: ";
			cin >> UserGuess;
			while (!validateGuess(UserGuess))
			{
				cout << "Invalid Guess. Try Again." << endl << endl;
				cout << UserName << " Guesses: ";
				cin >> UserGuess;
			}
			cout << endl << "Result: " << resultUserGuess(Answer, UserGuess) << endl << endl;

			if (resultUserGuess(Answer, UserGuess) == "3S 0B") {
				userWin = true;
				break;
			}

			// Computer Guess
			Guess = returnGuess(guesses);
			cout << "Computer guesses: " << Guess << endl << endl;

			cout << "Input result separated by spaces as [strike] [ball]: ";
			cin >> strike >> ball;
			if (strike == 3)
			{
				computerWin = true;
				break;
			}
			updateGuesses(&guesses, strike, ball, Guess, level);
			cout << endl << "Guesses left: " << guesses.size() << endl;
		}

		if (userWin)
		{
			cout << endl << UserName << " won in " << trycount << " Turns. Congratulation!" << endl;
			updateleaderboard(level, true, UserName, "cpu");
		}
		else if (computerWin)
		{
			cout << endl << "Computer won in " << trycount << " Turns." << endl;
			updateleaderboard(level, false, UserName, "cpu");
		}
	}

	// vs Player 2
	if (any1 == "2")
	{
		string Player1Answer, Player1Guess;
		string Player2Answer, Player2Guess;
		string player1, player2;
		int TryCount = 0;
		bool Player1Win = false, Player2Win = false;

		

		cout << endl << endl << "This is 2 player mode." << endl << endl;
		cout << "Instructions: " << endl;
		cout << "If both player guess the opponent's number in the same turn, then it is considered as a draw." << endl;
		cout << "The input will not be shown to the screen. So write down your number and guesses in a separate sheet of paper." << endl;
		cout << "Keep your number and your guesses confidential." << endl;

		printleaderboard(4);
		cout << endl << endl;


		cout << "Input your username player1: ";
		cin.ignore();
		cin.clear();
		getline(cin, player1);

		cout << "Input your username player2: ";
		getline(cin, player2);

		SetStdinEcho(false);

		// the number input
		cout << endl << "Input " << player1 << "'s number without showing the number to the opponent: ";
		cin >> Player1Answer;
		while (!validateGuess(Player1Answer))
		{
			cout << endl <<"The number is invalid. Try again." << endl << endl;
			cout << "Input " << player1 << "'s number without showing the number to the opponent: ";
			cin >> Player1Answer;
		}
		
		cout << endl << "Input "<< player2 << "'s number without showing the number to the opponent: ";
		cin >> Player2Answer;
		while (!validateGuess(Player2Answer))
		{
			cout << endl << "The number is invalid. Try again." << endl << endl;
			cout << "Input " << player2 << "'s number without showing the number to the opponent: ";
			cin >> Player2Answer;
		}

		// guess start
		while (!Player1Win && !Player2Win)
		{
			cout << endl << "Turn " << ++TryCount << endl;

			// Player1 Guess
			cout << player1 << " Guesses " << player2 << "'s number: "; cin >> Player1Guess;
			while (!validateGuess(Player1Guess))
			{
				cout << endl << "Invalid Guess. Try Again." << endl;
				cout << player1 << " Guesses " << player2 << "'s number: ";
				cin >> Player1Guess;
			}
			cout << endl << endl << "Result of Player1's Guess: " << resultUserGuess(Player2Answer, Player1Guess) << endl;

			if (resultUserGuess(Player2Answer , Player1Guess) == "3S 0B") {
				Player1Win = true;
			}
			
			// Player2
			cout << player2 << " Guesses " << player1 << "'s number: ";
			cin >> Player2Guess;
			while (!validateGuess(Player2Guess))
			{
				cout << "Invalid Guess. Try Again." << endl;
				cout << player2 << " Guesses " << player1 << "'s number: ";
				cin >> Player2Guess;
			}
			cout << endl << endl << "Result of " << player2 << "'s Guess: " << resultUserGuess(Player1Answer, Player2Guess) << endl;

			if (resultUserGuess(Player1Answer, Player2Guess) == "3S 0B") {
				Player2Win = true;
			}
		}

		SetStdinEcho(true);

		// one of them won
		if (Player1Win && Player2Win)
		{
			cout << endl << endl << "Draw on " << TryCount << " Turns." << endl;
		}
		else if (Player1Win)
		{
			cout << endl << player1 << " wins on " << TryCount << " Turns. Congratulations!" << endl;
			cout << "Player1's number was: " << Player1Answer << endl;
			updateleaderboard(4, true, player1, player2);
		}
		else
		{
			cout << endl << player2 << " wins on " << TryCount << " Turns. Congratulations!" << endl;
			cout << "Player2's number was: " << Player2Answer << endl;
			updateleaderboard(4, false, player1, player2);
		}
	}

	cout << "Exitting..." << endl;
	system("pause");
	return 0;
}

