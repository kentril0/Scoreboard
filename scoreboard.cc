/**
 * @file scoreboard.cc
 * @date 24.8.2018
 * @author Kentril Despair
 * @version 1.0
 * @brief Denfinitions of scoreboard terminal like app, with no GUI
 *	Scoreboard shows player scores and ranking a table-like manner.
 */

#include "scoreboard.h"
#include <algorithm>


/**
 * @brief Sets a player name
 * @param name Desired new name
 */
inline void Player::set_name(const std::string &name)
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
	if (num > MAX_SCORE)		
		score = MAX_SCORE;		// automatically sets to upper limit
	else if (num < MIN_SCORE)
		score = MIN_SCORE;		// automatically sets to lower limit

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
 * @param m_plyrs Maximum number of players that can be created
 * @param s_f Name of a file to which the table is saved instead of output
 * @param s_hf Name of a history file from which previous infomation is
 *				loaded.
 */
void Scoreboard::Scoreboard(int plyrs = 0, int s_max = HGHT_LIMIT,
							int m_plyrs = S_PLIMIT, 
							const std::string s_f = "", 
							const std::string s_hf = "")
{
	debug_msg("Scoreboard " << s_max << " limit:" << m_plyrs);

	if (plyrs > m_plyrs)
	{
		report_war("Only " << m_plyrs << " players will be created, change
			this limit later: set plimit N, where N is the new limit");
		plyrs = m_plyrs;
	}

	// init vector of players to plyrs number of players
	for(int i = 0; i < plyrs; i++)
		add_player();

	// init 
	show_max = s_max;			// maximum number of players shown 
	max_players = m_plyrs;		// maximum players that can be created

	// TODO mode file-saving if empty string
	save_f(s_f);				// initialize save file
	h_file(s_hf);				// initialize history file
}

/**
 * @brief Sets current number of shown players
 * @param num Maximum number of shown players
 */
inline void Scoreboard::set_show_max(int num)
{
	debug_info();

	if (num < 0 || num > USHRT_MAX)
		report_err("Incorrect number of maximum players shown");

	show_max = num;
}

/**
 * @brief Sets maximum number of players that can be created
 * @param num Maximum number of shown players
 */
inline void Scoreboard::set_max_players(int num)
{
	debug_info();

	if (num < 0 || num > USHRT_MAX)	// hard limit 
		report_err("Incorrect number of maximum players shown");

	int i = players.size()-1;
	while (i > num)			// remove players above limit
	{
		rm_player(i);		// TODO range delete
		i--;
	}

	max_players = num;
}

/**
 * @brief Adds a player to the scoreboard
 * 	Checks player limit, uniqueness of player's name, if not unique, then
 *	(N) is prepended, wher N is the number of that many times used name
 * @param name Name of the player
 * @param score Score of the player
 */
void Scoreboard::add_player(const std::string &name = "", int score = 0)
{
	debug_info();

	if (players.size() >= max_players)		// checking limit of players
		report_err("Cannot create another player, at limit!");
	
	if (name.length() > MAX_PNAME)			// max limit of chars exceeded
		report_err("Player name too long, maximum 32 characters!")

	char *p_name = new char[MAX_PNAME]();	// TODO exceptions
	std::memset(p_name, '\0', MAX_PNAME);
	char *null_pos;							// pointer to null position

	if (name.empty())	// default used - generate name
	{
		std::strcpy(p_name, "Player");
		null_pos = &p_name[6];
	}
	else				// copy name
	{
		std::strcpy(p_name, name.c_str());
		null_pos = &p_name[name.length()];	// TODO check
	}

	std::string aux;
	// checking uniqueness of player's name
	std::map<char *, int>::const_iterator it = players.find(p_name);
	for (int i = 1; it != players.end(); i++)
	{
		aux << "(" << i << ")";				// max (65534)
		std::strcpy(null_pos, aux.c_str());	// appending (N) 
		it = players.find(p_name);
	}

	players[p_name] = score;				// adding player

	sort_scb();								// need to sort
}

/**
 * @brief Gets a pointer reference to a player using his rank
 * @param rank A position in the table score system
 * @return Pointer to the player iterator
 */ 
inline Pl_it Scoreboard::get_player(int rank)
{
	debug_info();

	if (rank < 1 || rank > players.size())	// use exceptions TODO
		report_err("Incorrect player rank");
	
	std::set<std::pair<char *, int>>::iterator it = 
		std::next(pl_sort.begin(), rank-1);
	
	return players.find(it->name);
}

/**
 * @brief Gets a pointer reference to a player using his name
 * @param name Player's identifiable name
 * @return Pointer to the player iterator
 */ 
inline Pl_it Scoreboard::get_player(std::string &name)
{
	debug_info();

	if (name.empty() || name.length > PNAME_LIMIT)
		return nullptr;

	Pl_it it = players.find(name);
	if (it != players.end())
		return it;

	report_war("Player with that name does not exist");
	return nullptr;
}

/**
 * @brief Removes a player with certain rank
 * @param rank Rank of the player to be removed
 */
inline void Scoreboard::rm_player(int rank)
{
	debug_info();

	if (rank < 1 || rank > players.size())	// use exceptions TODO
		report_err("Incorrect player rank");
	
	std::set<std::pair<char *, int>>::iterator it = 
		std::next(pl_sort.begin(), rank-1);
	
	delete [](it->first);					// free the mem TODO check
	players.erase(it->first);
	pl_sort.erase(it);						// TODO sort again??
}

/**
 * @brief Removes a player with a certain name
 * @param Name of the player to be removed
 */
inline void Scoreboard::rm_player(std::string &name)
{
	debug_info();

	if (name.empty())
		return;
	
	std::map<char *, int>::iterator it = p_names.find(name);
	if (it != p_names.end())
	{
		pl_sort.erase(it->first);			// TODO pair needed??
		delete [](it->first);
		players.erase(it);
		return;
	}

	report_war("Player with that name does not exist");
}

/**
 * @brief Empties both structures
 */
inline void Scoreboard::rm_players()
{
	debug_info();

	for (std::map<char *, int>::iterator it = players.begin(); 
		it != players.end(); it++)
		delete [](it->first);				// delete alloc. strings

	players.clear();
	pl_sort.clear();
}

/**
 * @brief Renames player defined by his rank to new_name. No empty string
 *	and maximally 32 characters long string.
 * @param rank Rank of the player whoose name will be changed
 * @param new_name New name of the player
 */
inline void Scoreboard::rename_player(int rank, std::string &new_name)
{
	debug_info();

	if (new_name.empty() || new_name.length > MAX_PNAME)
		report_err("Incorrect new name specified")

	auto it = Pl_it Scoreboard::get_player(rank);	// checking rank
	if (!it)
		return;

	// checking uniqueness of player's name
	std::string aux;
	std::map<char *, int>::const_iterator a_it = players.find(new_name);
	for (int i = 1; a_it != players.end(); i++)
	{
		aux.clear();
		aux << new_name << "(" << i << ")";
		a_it = players.find(aux);
	}

	std::strcpy(it->first, aux);					// copy name over
}

/**
 * @brief Renames player defined by his name to a new_name
 * @param name Name of the player to be renamed
 * @param new_name A new name for the player
 */
inline void Scoreboard::rename_player(std::string &name, 
										std::string &new_name)
{
	debug_info();

	if (new_name.empty() || new_name.length > MAX_PNAME)
		report_err("Incorrect new name specified")

	auto it = Pl_it Scoreboard::get_player(new_name);	// checking name
	if (!it)
		return;

	// checking uniqueness of player's name
	std::string aux;
	std::map<char *, int>::const_iterator a_it = players.find(new_name);
	for (int i = 1; a_it != players.end(); i++)
	{
		aux.clear();
		aux << new_name << "(" << i << ")";
		a_it = players.find(aux);
	}

	std::strcpy(it->first, aux);					// copy name over
}

/**
 * @brief Adds a number to a player's score, identified by his rank
 * @param rank Rank of player
 * @param num Number added to the player's score (can be negative)
 */
inline void Scoreboard::add_pscore(int rank, int num)
{
	debug_info();

	auto it = get_player(rank);
	if (!it)
		return;

	if (num > MAX_SCORE)		
		num = MAX_SCORE;		// automatically sets to upper limit
	else if (num < MIN_SCORE)
		num = MIN_SCORE;		// automatically sets to lower limit

	it->second += num;

	sort_scb();					// need to sort again
}

/**
 * @brief Adds a number to a player's score, identified by his name
 * @param name Name of the player
 * @param num Number added to the player's score (can be negative)
 */
inline void Scoreboard::add_pscore(std::string &name, int num)
{
	debug_info();

	auto it = get_player(name);
	if (!it)
		return;

	if (num > MAX_SCORE)		
		num = MAX_SCORE;		// automatically sets to upper limit
	else if (num < MIN_SCORE)
		num = MIN_SCORE;		// automatically sets to lower limit

	it->second += num;

	sort_scb();					// need to sort again
}

/**
 * @brief Resets player's score to 0
 * @param rank Player's rank
 */
inline void Scoreboard::reset_pscore(int rank)
{
	debug_info();

	auto it = get_player(rank);
	if (!it)
		return;

	it->second = 0;

	sort_scb();					// need to sort again
}

/**
 * @brief Resets player's score to 0
 * @param name Player's name
 */
inline void Scoreboard::reset_pscore(std::string &name)
{
	debug_info();

	auto it = get_player(rank);
	if (!it)
		return;

	it->second = 0;

	sort_scb();					// need to sort again
}

/**
 * @brief Resets score of all players to zero
 */
inline void Scoreboard::reset_score()
{
	debug_info();

	for (auto it = players.begin(); it != players.end(); it++)
		it->second = 0;

	sort_scb();					// need to sort again
}
		
/**
 * @brief TODO
 */
inline bool Scoreboard::save_to_file(std::ostream file)
{
	debug_info();

}

/**
 * @brief TODO
 */
inline bool Scoreboard::load_players_from_file(std::istream file)
{
	debug_info();

}

/**
 * @brief TODO
 */
inline bool Scoreboard::load_history(std::istream file)
{
	debug_info();

}

/**
 * @brief
 */
inline void Scoreboard::print(std::ostream & strm = std::cout)
{
	debug_info();

	for (auto it = pl_sort.begin(); it != pl_sort.end(); it++)
		strm << "| " << it->first " | " << it->second " |\n";
}

/**
 * @brief Sorts the scoreboard players based on their score using
 *	set structure descending and aplhabetically when scores match
 */
inline void Scoreboard::sort_scb()
{
	debug_info();

	// lambda function object for comparing two pairs, 
	//	first values then keys if match
	Comparator compFunctor =	
		[](std::pair<char *, int> const &a, std::pair<char *, int> const &b)
		{
			return a.second != b.second ? a.second > b.second :
											a.first < b.first;
		};

	pl_sort(players.begin(), players.end(), compFunctor);
}
