#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "json.hpp"
#include "leaderboard.h"
#include <algorithm>

using namespace std;
using json = nlohmann::json;

template<typename A, typename B>
std::pair<B, A> flip_pair(const std::pair<A, B> &p)
{
	// switch pairs 
	return std::pair<B, A>(p.second, p.first);
}

template<typename A, typename B>
std::multimap<B, A> flip_map(const std::map<A, B> &src)
{
	// switch pairs for all items
	std::multimap<B, A> dst;
	std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
		flip_pair<A, B>);
	return dst;
}

void printleaderboard(int level)
{
	ifstream i("leaderboard.json");
	json j;
	i >> j;

	json part;

	// choose level
	switch (level)
	{
	case 1: part = j["beginner"];
		cout << endl << "Beginner leaderboard" << endl;
		break;
	case 2: part = j["medium"];
		cout << endl << "Medium leaderboard" << endl;
		break;
	case 3: part = j["hard"];
		cout << endl << "Hard leaderboard" << endl;
		break;
	case 4: part = j["pvp"];
		cout << endl << "pvp leaderboard" << endl;
		break;
	}

	auto playerlist_json = part.get<map <string, json>>();
	
	if (playerlist_json.size() == 0)
	{
		cout << "No Entry yet!" << endl << endl;
	}
	else
	{
		vector<player> playerlist;
		for (auto &item : playerlist_json)
		{
			player k;
			k.name = item.first;
			k.win = item.second.at(0);
			k.loss = item.second.at(1);
			playerlist.push_back(k);
		}

		// sort as win
		for(int p = 0; p < playerlist.size() - 1; p++)
		{
			for (int jk = 0; jk < playerlist.size() - 1 - p; jk++)
			{
				if (playerlist[jk].win < playerlist[jk+1].win)
				{
					auto temp = playerlist[jk];
					playerlist[jk] = playerlist[jk + 1];
					playerlist[jk + 1] = temp;
				}
			}
 		}

		// sort losses
		for (int p = 0; p < playerlist.size() - 1; p++)
		{
			for (int j = 0; j < playerlist.size() - 1 - p; j++)
			{
				// only when the win is same
				if (playerlist[j].win == playerlist[j+1].win)
				{
					// loss should go back
					if (playerlist[j].loss > playerlist[j+1].loss)
					{
						auto temp = playerlist[j];
						playerlist[j] = playerlist[j + 1];
						playerlist[j + 1] = temp;
					}
				}
			}
		}

		for (int i = 0; i < playerlist.size(); i++)
		{
			auto k = playerlist[i];
			cout << setw(12) << left << k.name << " : " << setw(3) << right << k.win << " wins " << k.loss << " losses" << endl;
		}

		/*
		 *	
		 *	flip value and key
			multimap<int, string> playersorted = flip_map(playerlist1);

			// iterate from the back
			multimap<int, string>::reverse_iterator rit;
			for (rit = playersorted.rbegin(); rit != playersorted.rend(); ++rit)
			{
				string player = rit->second;
				auto list = playerlist[player];
				cout << setw(12) << left << player << " : " << setw(3) << right << list[0] << " wins " << list[1] << " losses" << endl;
			}
		 */
		
	}
}

void updateleaderboard(int level, bool win, string player, string player2)
{
	ifstream i("leaderboard.json");
	json j;
	i >> j;
	
	json part;
	switch (level)
	{
	case 1: part = j["beginner"];
		break;
	case 2: part = j["medium"];
		break;
	case 3: part = j["hard"];
		break;
	case 4: part = j["pvp"];
	}

	auto leaderboard_parse = part.get<std::map <string, json>>();
	
	// value exist
	if (win)
	{
		// player1 already exist
		if (leaderboard_parse[player].size() != 0)
		{
			int k = leaderboard_parse[player].at(0);
			k += 1;
			leaderboard_parse[player].at(0) = k;
		}
		// not
		else
		{
			json j = "[ 1, 0 ]"_json;
			leaderboard_parse[player] = j;
		}

		if (player2 != "cpu")
		{
			//player 2 exist
			if (leaderboard_parse[player2].size() != 0)
			{
				int k = leaderboard_parse[player2].at(1);
				k += 1;
				leaderboard_parse[player2].at(1) = k;
			}
			// not
			else
			{
				json j = "[ 0, 1 ]"_json;
				leaderboard_parse[player2] = j;
			}
		}
	}
	else
	{
		if (leaderboard_parse[player].size() != 0)
		{
			int k = leaderboard_parse[player].at(1);
			k += 1;
			leaderboard_parse[player].at(1) = k;
		}
		else
		{
			json j = "[ 0, 1 ]"_json;
			leaderboard_parse[player] = j;
		}

		if (player2 != "cpu")
		{
			if (leaderboard_parse[player2].size() != 0)
			{
				int k = leaderboard_parse[player2].at(0);
				k += 1;
				leaderboard_parse[player2].at(0) = k;
			}
			else
			{
				json j = "[ 1, 0 ]"_json;
				leaderboard_parse[player2] = j;
			}
		}
	}


	part = json(leaderboard_parse);

	switch (level)
	{
	case 1: j["beginner"] = part;
		break;
	case 2: j["medium"] = part;
		break;
	case 3: j["hard"] = part;
		break;
	case 4: j["pvp"] = part;
	}

	ofstream o("leaderboard.json");
	o << j << endl;
}
