/**
 * @file scoreboard.h
 * @date 24. 8. 2018
 * @author Kentril Despair
 * @version 1.0
 * @brief Declarations of scoreboard program
 * 
 */


#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <iostream>
#include <fstream>
#include <climits>
#include <map>
#include <vector>
#include <functional>

// debugging macro
#ifndef DEBUG
#define debug_msg(...)

#define debug_info()

#else
#define debug_msg(x) do { std::cerr << __FUNCTION__ << ": " <<			\
								__LINE__ << ": " << x << std::endl; 	\
						} while(0)

#define debug_info() do { std::cerr << __FUNCTION__ << ": " <<			\
								__LINE__ << std::endl; } while(0)

#endif

#define report_err(x, y) do { std::cerr << "<Error>: " << x << std::endl; \
	return y; } while(0)

#define report_war(x) do { std::cerr << "<Warning>: " << x << std::endl; \
	} while(0)

#define LINE_BREAK std::cout << " " << std::string(w.ws_col-2, '-') <<	\
	std::endl;

/**
 * @brief An enum for all constants
 */
enum Consts
{
	// hard limit for number of players - USHRT_MAX
	S_PLIMIT = UCHAR_MAX,	// soft player limit
	H_PLIMIT = USHRT_MAX,	// hard player limit

	// that many players defined by the height of the terminal will be shown
	HGHT_LIMIT = -110,

	// score limits
	MAX_SCORE = 9999,
	MIN_SCORE = -9999,

	// name limits
	MAX_PNAME = 32,			// 40 chars, 32 + 7 optional + \0 - max length
	PNAME_LIMIT = 40,		// hard limit, also true size of string

	// terminal constants
	WIN_PADDING = 29		// window padding
};


typedef std::map<char *, int>::iterator Pl_it;


/**
 * @brief TODO DEPRECATED NOW TODO
 */
class Player
{
	std::string name;	///< Player name, by default "Player_%"
	int score;			///< Current score, by default 0
	
public:
	// implicit constructor
	Player(const std::string new_name, int sc = 0): name{new_name}, 
													score{sc} {}
	// name modifying methods
	void set_name(const std::string &name);
	std::string const *get_name();

	// score modifying methods
	void set_score(int num);
	int get_score();
	void inc_score();
	void dec_score();

	~Player() {}					// destructor
};

/**
 * @brief Sets a player name
 * @param name Desired new name
 */
inline void Player::set_name(const std::string &name)
{
	this->name = name;
}

/**
 * @brief Provides access to the name of player
 * @return Const pointer to name of player
 */
inline std::string const *Player::get_name()
{
	return &name;
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
		report_war("Minimum score reached");
}

// ----------------------------------------------------------------------

/**
 * @brief TODO
 */
class Scoreboard
{
		std::map<char *, int> players;	//< map of player names and scores
		std::vector<std::pair<char *, int>> pl_sort;
		int show_max;				///< How many players are shown
		unsigned int max_players;	///< Max. players to save info about
		std::filebuf save_f;		///< Can be printed to a file
		std::filebuf h_file;		///< History file saved players & scores
	public:
		// constructor
		Scoreboard(int plyrs = 0, int s_max = HGHT_LIMIT, 
					int m_plyrs = S_PLIMIT, 
					const std::string &s_f = std::string(),
					const std::string &s_hf = std::string());

		void set_show_max(int num);
		void set_max_players(int num);

		// player modification methods
		void add_player(const std::string &name = "", int score = 0);

		void rm_player(int rank);
		void rm_player(std::string &name);
		void rm_players();

		void rename_player(int rank, std::string &new_name);
		void rename_player(std::string &name, std::string &new_name);

		// score modification methods
		void add_pscore(int rank, int num = 1);
		void add_pscore(std::string &name, int num = 1);
		void reset_pscore(int rank);
		void reset_pscore(std::string &name);
		void reset_score();
		
		bool save_to_file(std::ostream file);
		bool load_players_from_file(std::istream file);
		bool load_history(std::istream file);

		void print(std::ostream & strm = std::cout);

		~Scoreboard() {	rm_players(); }			// destructor
	private:
		void sort_scb();				// sorting function for set pl_sort
		Pl_it get_player(int rank);
		Pl_it get_player(std::string &name);
};

/**
 * @brief Sets current number of shown players
 * @param num Maximum number of shown players
 */
inline void Scoreboard::set_show_max(int num)
{
	debug_info();

	if (num < 0 || num > USHRT_MAX)
		report_err("Incorrect number of maximum players shown", void());

	show_max = num;
}

/**
 * @brief Gets a pointer reference to a player using his rank
 * @param rank A position in the table score system
 * @return Pointer to the player iterator
 */ 
inline Pl_it Scoreboard::get_player(int rank)
{
	debug_info();

	// use exceptions TODO
	if ( (rank < 1) || (static_cast<unsigned int>(rank) > players.size()))	
		report_err("Incorrect player rank", players.end());
	
	std::vector<std::pair<char *, int>>::iterator it = 
		std::next(pl_sort.begin(), rank-1);
	
	return players.find(it->first);
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
 * @brief Resets score of all players to zero
 */
inline void Scoreboard::reset_score()
{
	debug_info();

	for (auto it = players.begin(); it != players.end(); it++)
		it->second = 0;

	sort_scb();					// need to sort again
}
		
#endif	// include SCOREBOARD_H
