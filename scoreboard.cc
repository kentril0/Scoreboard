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
#include <cstring>
#include <sstream>		// std::ostringstream
#include <sys/ioctl.h>	// get terminal
#include <unistd.h>


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
Scoreboard::Scoreboard(int plyrs, int s_max,
							int m_plyrs, 
							const std::string &s_f, 
							const std::string &s_hf)
{
	debug_msg("max show: " << s_max);
	debug_msg("limit: " << m_plyrs);

	if (plyrs > m_plyrs)
	{
		report_war("Only " << m_plyrs << " players will be created, change"
			" this limit later: set plimit N, where N is the new limit");
		plyrs = m_plyrs;
	}

	// init vector of players to plyrs number of players
	for(int i = 0; i < plyrs; i++)
		add_player();

	// init 
	show_max = s_max;			// maximum number of players shown 
	max_players = m_plyrs;		// maximum players that can be created

	// TODO mode file-saving if empty string
	if (!s_f.empty())		// TODO exceptions
		save_f.open(s_f, std::ios::out);	// initialize save file

	if (!s_hf.empty())
		h_file.open(s_hf, std::ios::out);	// initialize history file
}

/**
 * @brief Sets maximum number of players that can be created
 * @param num Maximum number of shown players
 */
void Scoreboard::set_max_players(int num)
{
	debug_info();

	if (num < 0 || num > USHRT_MAX)	// hard limit 
		report_err("Incorrect number of maximum players shown", void());

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
void Scoreboard::add_player(const std::string &name, int score)
{
	debug_info();

	if (players.size() >= max_players)		// checking limit of players
		report_err("Cannot create another player, at limit!", void());
	
	if (name.length() > MAX_PNAME)			// max limit of chars exceeded
		report_err("Player name too long, maximum 32 characters!", void());

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

	std::ostringstream aux;
	// checking uniqueness of player's name
	std::map<char *, int>::const_iterator it = players.find(p_name);
	for (int i = 1; it != players.end(); i++)
	{
		aux.clear();
		aux << "(" << i << ")";				// max (65534)
		std::strcpy(null_pos, aux.str().c_str());	// appending (N) 
		it = players.find(p_name);
	}

	players[p_name] = score;				// adding player

	sort_scb();								// need to sort
}

/**
 * @brief Gets a pointer reference to a player using his name
 * @param name Player's identifiable name
 * @return Pointer to the player iterator
 */ 
Pl_it Scoreboard::get_player(std::string &name)
{
	debug_info();

	if (name.empty() || name.length() > PNAME_LIMIT)
		return players.end();

	char *aux_str = new char[name.length()+1]();	// const problems
	std::strcpy(aux_str, name.c_str());

	Pl_it it = players.find(aux_str);
	delete [](aux_str);
	if (it != players.end())
		return it;

	report_war("Player with that name does not exist");
	return players.end();
}

/**
 * @brief Removes a player with certain rank
 * @param rank Rank of the player to be removed
 */
void Scoreboard::rm_player(int rank)
{
	debug_info();
	
	// use exceptions TODO
	if (rank < 1 || (static_cast<unsigned int>(rank) > players.size()))
		report_err("Incorrect player rank", void());
	
	std::vector<std::pair<char *, int>>::iterator it = 
		std::next(pl_sort.begin(), rank-1);
	
	delete [](it->first);					// free the mem TODO check
	players.erase(it->first);
	pl_sort.erase(it);						// TODO sort again??
}

/**
 * @brief Removes a player with a certain name
 * @param Name of the player to be removed
 */
void Scoreboard::rm_player(std::string &name)
{
	debug_info();

	if (name.empty())
		return;
	
	char *aux_str = new char[name.length()+1]();	// const problems
	std::strcpy(aux_str, name.c_str());

	Pl_it it = players.find(aux_str);
	delete [](aux_str);
	if (it != players.end())
	{
		delete [](it->first);
		players.erase(it);
		sort_scb();
		return;
	}

	report_war("Player with that name does not exist");
}

/**
 * @brief Renames player defined by his rank to new_name. No empty string
 *	and maximally 32 characters long string.
 * @param rank Rank of the player whoose name will be changed
 * @param new_name New name of the player
 */
void Scoreboard::rename_player(int rank, std::string &new_name)
{
	debug_info();

	if (new_name.empty() || new_name.length() > MAX_PNAME)
		report_err("Incorrect new name specified", void());

	Pl_it it = get_player(rank);	// checking rank
	if (it == players.end())
		return;

	// checking uniqueness of player's name
	char *aux_str = new char[PNAME_LIMIT]();	// const problems
	std::memset(aux_str, '\0', MAX_PNAME);
	std::strcpy(aux_str, new_name.c_str());

	std::ostringstream aux;
	std::map<char *, int>::iterator a_it = players.find(aux_str);
	for (int i = 1; a_it != players.end(); i++)
	{
		aux.clear();
		std::memset(aux_str, '\0', MAX_PNAME);
		aux << new_name << "(" << i << ")";
		std::strcpy(aux_str, aux.str().c_str());
		a_it = players.find(aux_str);
	}

	std::strcpy(it->first, aux_str);		// copy name over
	sort_scb();								// need to sort
}

/**
 * @brief Renames player defined by his name to a new_name
 * @param name Name of the player to be renamed
 * @param new_name A new name for the player
 */
void Scoreboard::rename_player(std::string &name, 
										std::string &new_name)
{
	debug_info();

	if (new_name.empty() || new_name.length() > MAX_PNAME)
		report_err("Incorrect new name specified", void());

	Pl_it it = get_player(name);		// checking name
	if (it == players.end())
		return;

	// checking uniqueness of player's name
	char *aux_str = new char[PNAME_LIMIT]();	// const problems
	std::memset(aux_str, '\0', MAX_PNAME);
	std::strcpy(aux_str, new_name.c_str());

	std::ostringstream aux;
	std::map<char *, int>::iterator a_it = players.find(aux_str);
	for (int i = 1; a_it != players.end(); i++)
	{
		aux.clear();
		std::memset(aux_str, '\0', MAX_PNAME);
		aux << new_name << "(" << i << ")";
		std::strcpy(aux_str, aux.str().c_str());
		a_it = players.find(aux_str);
	}

	std::strcpy(it->first, aux_str);		// copy name over
	sort_scb();								// need to sort
}

/**
 * @brief Adds a number to a player's score, identified by his rank
 * @param rank Rank of player
 * @param num Number added to the player's score (can be negative)
 */
void Scoreboard::add_pscore(int rank, int num)
{
	debug_info();

	auto it = get_player(rank);
	if (it == players.end())
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
void Scoreboard::add_pscore(std::string &name, int num)
{
	debug_info();

	auto it = get_player(name);
	if (it == players.end())
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
void Scoreboard::reset_pscore(int rank)
{
	debug_info();

	auto it = get_player(rank);
	if (it == players.end())
		return;

	it->second = 0;

	sort_scb();					// need to sort again
}

/**
 * @brief Resets player's score to 0
 * @param name Player's name
 */
void Scoreboard::reset_pscore(std::string &name)
{
	debug_info();

	auto it = get_player(name);
	if (it == players.end())
		return;

	it->second = 0;

	sort_scb();					// need to sort again
}
	
/**
 * @brief TODO
 */
bool Scoreboard::save_to_file(std::ostream file)
{
	debug_info();
	
	(void)file;

	return false;
}

/**
 * @brief TODO
 */
bool Scoreboard::load_players_from_file(std::istream file)
{
	debug_info();
	
	(void)file;

	return false;
}

/**
 * @brief TODO
 */
bool Scoreboard::load_history(std::istream file)
{
	debug_info();

	(void)file;

	return false;
}

/**
 * @brief Prints the actual scores, players and ranking
 * 	Layout:
 *  _______________________________________________________________________
 * | RANK	| PLAYER NAME									| SCORE		   |
 *	-----------------------------------------------------------------------
 * | 1.		| Dudefish										| 22		   |
 * .
 * .
 * .
 * | 23.	| Kentril										| -20		   |
 *  -----------------------------------------------------------------------
 */
void Scoreboard::print(std::ostream & strm)
{
	debug_info();

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	// printing header TODO consider using std AFTER to check any problems
	strm << " " << std::string(w.ws_col-2, '_') << std::endl <<
		"| RANK\t| PLAYER NAME " <<
		std::string(w.ws_col-WIN_PADDING, ' ') << "| SCORE |" << std::endl;

	LINE_BREAK;

	int i = 1;
	for (auto it = pl_sort.begin(); it != pl_sort.end(); it++)
	{
		strm << "| " << i << ".\t|" << it->first << " " << 
			std::string(w.ws_col-19-strlen(it->first), ' ') << "| " << 
			it->second << " |" << std::endl;
		LINE_BREAK;
		i++;
	}
}

/**
 * @brief Sorts the scoreboard players based on their score using
 *	set structure descending and aplhabetically when scores match
 */
void Scoreboard::sort_scb()
{
	debug_info();

	// lambda function object for comparing two pairs, 
	//	first values then keys if match
/*	Comparator compFunctor =	
		[](std::pair<char *, int> const &a, std::pair<char *, int> const &b)
		{
			return a.second != b.second ? a.second > b.second :
											a.first < b.first;
		};

*/
//	pl_sort (players.begin(), players.end(), compFunctor);
	// "shallow" copy
//	std::transform(players.begin(), players.end(), std::inserter(pl_sort, 
//		pl_sort.begin()), compFunctor);


	std::copy(players.begin(), players.end(),
		std::back_inserter<std::vector<std::pair<char *, int>>>(pl_sort));

	std::sort(pl_sort.begin(), pl_sort.end(), 
				[](std::pair<char *, int> const &a, 
					std::pair<char *, int> const &b)
					{
						return a.second != b.second ? a.second > b.second :
											a.first < b.first;
					});
	
	
}
