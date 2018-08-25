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

#include <string>
#include <vector>
#include <iostream>
#include <climits>

// debugging macro
#ifndef DEBUG
#define DEBUG_PRINT(...)
#else
#define DEBUG_PRINT(x) do { std::cerr << __FUNCTION__ << ":" <<			\
								<< __LINE__ << ": " << x << std::endl; 	\
						} while(0)
#endif

// hard limit for number of players - INT_MAX
const int PLIMIT = UCHAR_MAX;	// soft player limit

// show that many players defined by the height of the terminal
const int HGHT_LIMIT = -110;


/**
 * @brief
 */
class Player
{
	std::string name;	///< Player name, by default "Player_%"
	int score;			///< Current score, by default 0
public:
	Player();			// implicit constructor
	Player(std::string name = "Unnamed", int score = 0);

	// name modifying methods
	void set_name(std::string name);
	std::string const* get_name();	// TODO TEST CONST POINTER

	// score modifying methods
	void set_score(int num);
	int get_score();
	void inc_score();
	void dec_score();

	~Player();			// destructor
};

/**
 * @brief
 */
class Scoreboard
{
		std::vector<Player> players;///< Vector of players
		int show_max;				///< How many players are shown
		int max_players;			///< Max. players to save info about
		std::ostream save_f;		///< Can be printed to a file
		std::ostream h_file;		///< History file saved players & scores
	public:
		// constructor
		Scoreboard(int s_max = HGHT_LIMIT, int m_players = PLIMIT);

		void set_show_max(int num);
		void set_max_players(int num);

		// player modification methods
		void add_player(std::string name = "", int score = 0);
		void add_players(int num);		// TODO maybe just one function
		Player const *get_player(int rank);
		Player const *get_player(std::string name);
		void rm_player(int rank);
		void rm_player(std::string name);
		void rm_players();
		void rename_player(int rank, std::string new_name);
		void rename_player(std::string name, std::string new_name);

		// score modification methods
		void add_pscore(int rank, int num);
		void add_pscore(std::string name, int num);
		void reset_pscore(int rank);
		void reset_pscore(std::string name);
		void reset_score();
		
		bool save_to_file(std::ostream file);
		bool load_players_from_file(std::istream file);
		bool load_history(std::istream file);

		void print(std::ostream & strm = std::cout);

		~Scoreboard();			// destructor
};

#endif	// include SCOREBOARD_H
