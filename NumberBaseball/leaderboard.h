#pragma once
#include <iostream>
#include "json.hpp"

using namespace std;

void printleaderboard(int level);
void updateleaderboard(int level, bool win, string player, string player2);

class player
{
public:
	string name;
	int win;
	int loss;
};