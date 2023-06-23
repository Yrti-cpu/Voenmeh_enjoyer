#pragma once
#include <string>
class Records
{
public:
	Records(int points, std::string player_name)
	{
		this->points = points;
		this->player_name = player_name;
	}

	int get_points() { return points; }
	std::string get_player_name() { return player_name; }
private:
	int points;
	std::string player_name;

};

