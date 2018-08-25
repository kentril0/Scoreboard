/**
 * @file scoreboard.cc
 * @date 24.8.2018
 * @author Kentril Despair
 * @version 1.0
 * @brief Denfinitions of scoreboard terminal like app, with no GUI
 *	Scoreboard shows player scores and ranking a table-like manner.
 */

#include "scoreboard.h"


/**
 * @brief Sets a player name
 * @param name Desired new name
 */
inline void Player::set_name(const std::string name)
{
	this->name{name};
}

/**
 * @brief Provides access to the name of player
 * @return Const pointer to name of player
 */
inline std::string *Player::get_name() const
{
	return &name;	// TODO const
}

/**
 * @brief Sets score of a player
 * @param num New score of player
 */
inline void Player::set_score(int num)
{
	if (num > MAX_SCORE)		// automatically sets to upper limit
		score = MAX_SCORE;
	else if (num < MIN_SCORE)	// automatically sets to lower limit
		score = MIN_SCORE;

	score = num;
}

/**
 * @brief Get player score
 * @return player score
 */
inline int Player::get_score()
{
	return score;
}

/**
 * @brief Increments player score by one
 */
inline void Player::inc_score()
{
	if (score < MAX_SCORE)
		score++;
	else
		report_war("Maximum score reached");
}

/**
 * @brief Decrements player score by one
 */
inline void Player::dec_score()
{
	if (score > MIN_SCORE)
		score--;
	else
		report_war("Minimum score reached")
}

/* ------------------------------------------------------------ */

/**
 * @brief Implicit constructor of Scoreboard
 *	Based on defined arguments an output mode is devised:
 *	If s_f is not empty string than -> saving to a file instead of std out.
 * TODO
 * @param plyrs Number of players to initialze vector with
 * @param s_max Number of players shown at a time in a table
 * @param m_players Maximum number of players that can be created
 * @param s_f Name of a file to which the table is saved instead of output
 * @param s_hf Name of a history file from which previous infomation is
 *				loaded.
 */
void Scoreboard::Scoreboard(int plyrs = 0, int s_max = HGHT_LIMIT,
							int m_players = PLIMIT, 
							const std::string s_f = "", 
							const std::string s_hf = "")
{
	debug_msg("Scoreboard " << s_max << " limit:" << m_players);

	if (plyrs > m_players)
	{
		report_war("Only " << m_players << " players will be created, change
			this limit later: set plimit N, where N is the new limit");
		plyrs = m_players;
	}

	// init vector of players to plyrs number of players
	for(int i = 0; i < plyrs; i++)
	{
		Player pl("Player_" << i+1);
		players.push_back(pl);
	}

	// init 
	show_max = s_max;			// maximum number of players shown 
	max_players = m_players;	// maximum players that can be created
	save_f(s_f);				// initialize save file
	h_file(s_hf);				// initialize history file
}

/**
 * @brief Sets current number of shown players
 * @param num Maximum number of shown players
 */
inline void Scoreboard::set_show_max(int num)
{
	if (num < 0)
		report_err("Incorrect number of maximum players shown");

	show_max = num;
}

/**
 * @brief Sets maximum number of players that can be created
 * @param num Maximum number of shown players
 */
inline void Scoreboard::set_max_players(int num)
{
	if (num < 0)
		report_err("Incorrect number of maximum players shown");

	while(players.size() > num)	// remove players above limit
		players.pop_back();

	max_players = num;
}

/**
 * @brief Adds a player to the scoreboard
 * @param name Name of the player
 * @param score Score of the player
 */
void Scoreboard::add_player(std::string name = "", int score = 0)
{
	if (players.size() >= max_players)
		report_err("Cannot create another player, at limit!");
		
	if (name.empty())	// default used - generate name
		name{"Player_" << players.size()}; // TODO for now just some number

	Player pl("Player_" << i+1, score);

	players.push_back(pl);
}

/**
 * @brief Gets a pointer reference to a player using his rank
 * @param rank A position in the table score system
 * @return Const pointer to the player
 */ 
inline Player const *Scoreboard::get_player(int rank)
{
	if (rank < 1 || rank > players.size())	// use exceptions TODO
		report_err("Incorrect player rank");
	
	return players.at(rank-1);
}

/**
 * @brief Gets a pointer reference to a player using his name
 * @param name Player's identifiable name
 * @return Const pointer to the player
 */ 
Player const *Scoreboard::get_player(std::string name)
{
	// TODO a map of names?

}

/**
 * @brief
 */
inline void Scoreboard::rm_player(int rank)
{

}

/**
 * @brief
 */
inline void Scoreboard::rm_player(std::string name)
{

}

/**
 * @brief
 */
inline void Scoreboard::rm_players()
{

}

/**
 * @brief
 */
inline void Scoreboard::rename_player(int rank, std::string new_name)
{

}

/**
 * @brief
 */
inline void Scoreboard::rename_player(std::string name, std::string new_name)
{

}

/**
 * @brief
 */
inline void Scoreboard::add_pscore(int rank, int num)
{

}

/**
 * @brief
 */
inline void Scoreboard::add_pscore(std::string name, int num)
{

}

/**
 * @brief
 */
inline void Scoreboard::reset_pscore(int rank)
{

}

/**
 * @brief
 */
inline void Scoreboard::reset_pscore(std::string name)
{

}

/**
 * @brief
 */
inline void Scoreboard::reset_score()
{

}
		
/**
 * @brief
 */
inline bool Scoreboard::save_to_file(std::ostream file)
{

}

/**
 * @brief
 */
inline bool Scoreboard::load_players_from_file(std::istream file)
{

}

/**
 * @brief
 */
inline bool Scoreboard::load_history(std::istream file)
{

}

/**
 * @brief
 */
inline void Scoreboard::print(std::ostream & strm = std::cout)
{

}
