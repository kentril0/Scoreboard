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

#include <vector>
#include <iostream>
#include <climits>

// debugging macro
#ifndef DEBUG
#define debug_msg(...)
#else
#define debug_msg(x) do { std::cerr << __FUNCTION__ << ":" <<			\
								<< __LINE__ << ": " << x << std::endl; 	\
						} while(0)
#endif

#define report_err(x) do { std::cerr << "<Error>: " << x << std::endl; \
	return; } while(0)

#define report_war(x) do { std::cerr << "<Warning>: " << x << std::endl; \
	} while(0)


// hard limit for number of players - INT_MAX
const int PLIMIT = UCHAR_MAX;	// soft player limit

// show that many players defined by the height of the terminal
const int HGHT_LIMIT = -110;

// score limits
const int MAX_SCORE = 999;
const int MIN_SCORE = -999;


/**
 * @brief
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
	void set_name(const std::string name);
	std::string *get_name() const;	// TODO TEST CONST POINTER

	// score modifying methods
	void set_score(int num);
	int get_score();
	void inc_score();
	void dec_score();

	~Player() {}					// destructor
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
		Scoreboard(int plyrs = 0, int s_max = HGHT_LIMIT, 
					int m_players = PLIMIT, const std::string s_f = "",
					const std::string s_hf = "");

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
