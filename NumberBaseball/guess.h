#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// For USER
bool validateGuess(const string& cs);
string resultUserGuess(string answer, string guess);

// For CPU
// random number generator for cpu
string numberGenerator();
// Guess of the computer
string returnGuess(vector<string> k);
// Update the guess array
void updateGuesses(vector<string>* k, int strike, int ball, string guess, int level);
// Initialize the guess array
void createGuesses(vector<string>* k);