#include <iostream>
#include <string>
#include <time.h>
#include "guess.h"
#include <vector>

using namespace std;

// For Users
string resultUserGuess(string answer, string guess)
{
	int ball = 0, strike = 0;
	for (int i = 0; i < 3; i++)
	{
		if (answer.find(guess[i]) != string::npos)
		{
			if (answer.find(guess[i]) == i) strike++;
			else ball++;
		}
	}
	string re = to_string(strike) + "S " + to_string(ball) + "B";
	return re;
}

bool validateGuess(const string& cs)
{
	if (cs.size() != 3) return false;
	else
	{
		// check numeric
		for (int i = 0; i < 3; i++)
		{
			if (!isdigit(cs[i])) return false;
		}
		// check duplicate
		if (cs[0] == cs[1] || cs[1] == cs[2] || cs[0] == cs[2]) return false;
	}
	return true;
}

// For CPU
string numberGenerator()
{
	string answer;
	int i;
	srand(time(NULL));
	while (1)
	{
		for (i = 0; i<3; i++)
		{
			bool duplicate = false;
			char k = rand() % 10 + 0x30;
			for (int j = 0; j < i; j++)
			{
				while (answer[j] == k)
				{
					k = rand() % 10 + 0x30;
				}
			}
			answer += k;
		}
		return string(answer);
	}
}

void createGuesses(vector<string>* guesses)
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (j == i)
			{
				continue;
			}
			for (int k = 0; k < 10; k++)
			{
				if (k == i || k == j)
				{
					continue;
				}
				string guess = to_string(i) + to_string(j) + to_string(k);
				guesses->push_back(guess);
			}
		}
	}
}

string returnGuess(vector<string> guesses)
{
	srand(time(NULL));
	if (guesses.size() == 1)
	{
		string guess = guesses[0];
		return guess;
	}
	else if (guesses.size())
	{
		int k = rand() % guesses.size();
		string guess = guesses[k];
		return guess;
	}
	else
	{
		return "Error: something is wrong...";
	}
}

void updateGuesses(vector<string>* k, int strike, int ball, string guess, int level)
{
	char a = guess[0], b = guess[1], c = guess[2];
	// 0 0
	if (strike == 0 && ball == 0)
	{
		int i = 0;
		while(i < k->size())
		{
			string n = (*k)[i];
			// even one --> delete 
			if (n.find(a) != string::npos || n.find(b) != string::npos || n.find(c) != string::npos)
			{
				k->erase(k->begin() + i);
				if (level == 1)
				{
					i += 2;
				}
				if (level == 2)
				{
					i++;
				}
			}
			// other than that pass
			else i++;
		}
	}
	else if (strike == 0 && ball == 1)
	{
		int i = 0; 
		while(i < k->size())
		{
			string n = (*k)[i];
			// 하나만 있으면 살림
			// a exist
			if (n.find(a) != string::npos && n.find(b) == string::npos && n.find(c) == string::npos)
			{
				// a can't be in the first spot
				
				if (n.find(a) == 0) k->erase(k->begin() + i);
				else i++;

				if (level == 1)
				{
					i++;
				}
				else if (level == 2)
				{
					i++;
				}

			}
			// b exist
			else if (n.find(a) == string::npos && n.find(b) != string::npos && n.find(c) == string::npos)
			{
				if (n.find(b) == 1) k->erase(k->begin() + i);
				else i++;
				if (level == 1)
				{
					i++;
				}
				if (level == 2)
				{
					i++;
				}

			}
			// c exist
			else if (n.find(a) == string::npos && n.find(b) == string::npos && n.find(c) != string::npos)
			{

				if (n.find(c) == 2) k->erase(k->begin() + i);
				else i++;
				if (level == 1)
				{
					i++;
				}

			}
			// nothing exists? --> delete
			else
			{
				k->erase(k->begin() + i);
				if (level == 2)
				{
					i+=2;
				}

			}
		}
	}
	else if (strike == 0 && ball == 2)
	{
		int i = 0; 
		while(i < k->size())
		{
			string n = (*k)[i];
			// a and b exist
			if (n.find(a) != string::npos && n.find(b) != string::npos && n.find(c) == string::npos)
			{
				
				// a can't be in the first spot
				if (n.find(a) == 0) k->erase(k->begin() + i);
				else if (n.find(b) == 1) k->erase(k->begin() + i);
				else i++;

				if (level == 1)
				{
					i++;
				}
			}
			// b and c exist
			else if (n.find(a) == string::npos && n.find(b) != string::npos && n.find(c) != string::npos)
			{
				if (n.find(b) == 1) k->erase(k->begin() + i);
				else if (n.find(c) == 2) k->erase(k->begin() + i);
				else i++;

				if (level == 1)
				{
					i+=2;
				}
				else if (level == 2)
				{
					i++;
				}
			}
			// c and a exist
			else if (n.find(a) != string::npos && n.find(b) == string::npos && n.find(c) != string::npos)
			{
				if (n.find(c) == 2) k->erase(k->begin() + i);
				else if (n.find(a) == 0) k->erase(k->begin() + i);
				else i++;

				if (level == 1)
				{
					i+=2;
				}
			}
			// nothing exists? --> delete
			else
			{
				
				k->erase(k->begin() + i);
				if (level == 2)
				{
					i++;
				}
				if (level == 1)
				{
					i++;
				}
			}
		}
	}
	else if (strike == 0 && ball == 3)
	{
		int i = 0; 
		while(i < k->size())
		{
			string n = (*k)[i];
			// all three should exist
			if (n.find(a) == string::npos || n.find(b) == string::npos || n.find(c) == string::npos)
			{
				// if not? delete
				k->erase(k->begin() + i);
				if (level == 1)
				{
					i+=2;
				}
				else if (level == 2)
				{
					i++;
				}
			}
			// if one of the location is same
			else if (n.find(a) == 0 || n.find(b) == 1 || n.find(c) == 2) k->erase(k->begin() + i);
			// pass
			else i++;
		}
	}
	else if (strike == 1 && ball == 0)
	{
		int i = 0;
		while(i < k->size())
		{
			string n = (*k)[i];
			// 하나만 있으면 살림
			// a only exist
			if (n.find(a) != string::npos && n.find(b) == string::npos && n.find(c) == string::npos)
			{
				// a should be in the first spot
				if (n.find(a) != 0) k->erase(k->begin() + i);
				else i++;
				if (level == 1)
				{
					i+=2;
				}

			}
			// b only exist
			else if (n.find(a) == string::npos && n.find(b) != string::npos && n.find(c) == string::npos)
			{

				// b should be in the first spot
				if (n.find(b) != 1) k->erase(k->begin() + i);
				else i++;
				if (level == 1)
				{
					i++;
				}
				else if (level == 2)
				{
					i++;
				}

			}
			// c only exist
			else if (n.find(a) == string::npos && n.find(b) == string::npos && n.find(c) != string::npos)
			{
				// c should be in the first spot
				if (n.find(c) != 2) k->erase(k->begin() + i);
				else i++;
				if (level == 1)
				{
					i++;
				}


			}
			// nothing exists? --> delete
			else
			{
				k->erase(k->begin() + i);
				if (level == 1)
				{
					i+=2;
				}

			}
		}
	}
	else if (strike == 1 && ball == 1)
	{
		int i = 0; 
		while(i < k->size())
		{
			string n = (*k)[i];
			// a and b exist
			if (n.find(a) != string::npos && n.find(b) != string::npos && n.find(c) == string::npos)
			{
				if (n.find(a) == 0 && n.find(b) == 1) k->erase(k->begin() + i);
				else if (n.find(a) != 0 && n.find(b) != 1) k->erase(k->begin() + i);
				else i++;

				if (level == 1)
				{
					i++;
				}

			}
			// b and c exist
			else if (n.find(a) == string::npos && n.find(b) != string::npos && n.find(c) != string::npos)
			{

				if (n.find(c) == 2 && n.find(b) == 1) k->erase(k->begin() + i);
				else if (n.find(c) != 2 && n.find(b) != 1) k->erase(k->begin() + i);
				else i++;
				if (level == 1)
				{
					i+=2;
				}
				else if (level == 2)
				{
					i++;
				}

			}
			// c and a exist
			else if (n.find(a) != string::npos && n.find(b) == string::npos && n.find(c) != string::npos)
			{

				if (n.find(a) == 0 && n.find(c) == 2) k->erase(k->begin() + i);
				else if (n.find(a) != 0 && n.find(c) != 2) k->erase(k->begin() + i);
				else i++;
				if (level == 1)
				{
					i+=2;
				}

			}
			// nothing exists? --> delete
			else
			{
				k->erase(k->begin() + i);
				if (level == 1)
				{
					i+=2;
				}
				else if (level == 2)
				{
					i++;
				}
			}
		}
	}
	else if (strike == 1 && ball == 2)
	{
		int i = 0; 
		while(i < k->size())
		{
			string n = (*k)[i];
			// all three should exist
			if (n.find(a) != string::npos && n.find(b) != string::npos && n.find(c) != string::npos)
			{

				if (n.find(a) == 0 && n.find(b) == 1) k->erase(k->begin() + i);
				else i++;
				if (level == 1)
				{
					i++;
				}

			}
			else
			{
				k->erase(k->begin() + i);
				if (level == 1)
				{
					i++;
				}
				else if (level == 2)
				{
					i++;
				}
			}
		}
	}
	else if (strike == 2 && ball == 0)
	{
		int i = 0;
		while(i < k->size())
		{
			string n = (*k)[i];
			// a and b exist
			if (n.find(a) != string::npos && n.find(b) != string::npos && n.find(c) == string::npos)
			{
				// a and b should be in the their spot
				if (n.find(a) != 0) k->erase(k->begin() + i);
				else if (n.find(b) != 1) k->erase(k->begin() + i);
				else i++;
				if (level == 1)
				{
					i+=2;
				}

			}
			// b and c exist
			else if (n.find(a) == string::npos && n.find(b) != string::npos && n.find(c) != string::npos)
			{

				if (n.find(b) != 1) k->erase(k->begin() + i);
				else if (n.find(c) != 2) k->erase(k->begin() + i);
				else i++;
				if (level == 2)
				{
					i++;
				}

			}
			// c and a exist
			else if (n.find(a) != string::npos && n.find(b) == string::npos && n.find(c) != string::npos)
			{
				if (n.find(c) != 2) k->erase(k->begin() + i);
				else if (n.find(a) != 0) k->erase(k->begin() + i);
				else i++;
				if (level == 1)
				{
					i++;
				}

			}
			// nothing exists? --> delete
			else
			{
				k->erase(k->begin() + i);
				if (level == 1)
				{
					i+=2;
				}
				else if (level == 2)
				{
					i++;
				}

			}
		}
	}
	else
	{
		cout << "Invalid result. Check result again and input again." << endl;
		cout << "Input Result: ";
		cin >> strike >> ball;
		updateGuesses(k, strike, ball, guess, level);
	}
}

