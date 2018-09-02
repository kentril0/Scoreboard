/**
 * @file interface.cc
 * @date 31.08.2018
 * @author Kentril Despair
 * @brief TODO
 *
 */

#include "interface.h"
#include "scoreboard.h"
#include <unistd.h>
#include <cctype>
#include <unordered_map>
#include <algorithm>

static std::vector<std::string> v_exstr;
static std::unordered_map<std::string, user_cmnds> m_cmd_parse;
Scoreboard scb;

/**
 * @brief Initializes map 
 */
static std::unordered_map<std::string, user_cmnds> m_cmd_init()
{
	// initialized map with appropriate keywords - commands and their
	// codes
	std::unordered_map<std::string, user_cmnds> m_aux ( 
		{{"print", UC_PRINT}, {"scoreboard", UC_SCOREBOARD}, 
		{"show", UC_SHOW}, {"score", UC_SCORE}, {"player", UC_PLAYER}, 
		{"win", UC_WIN}, {"loss", UC_LOSS}, {"set", UC_SET}, 
		{"save", UC_SAVE}, {"load", UC_LOAD}, {"add", SC_ADD}, 
		{"remove", SC_REMOVE}, {"rename", SC_RENAME}, {"reset", SC_RESET}, 
		{"max", SC_MAX}, {"file", SC_FILE}, {"history", SC_HISTORY}, 
		{"players", SC_PLAYERS}, {"all", SC_ALL}, {"help", UC_HELP},
		{"exit", UC_EXIT}});

	return m_aux;
}

/**
 * @brief Splits string into a vector of string based on whitespace
 * @param str String to be split
 */
static void split_str(std::string &str)
{
	std::string aux;
	v_exstr.clear();	// delete any previous strings
	int i = 0, j = 0;

	for (std::string::iterator it = str.begin(); it != str.end(); it++, i++)
	{
		if ( std::isspace(*it) )		// char is whitespace
		{
			aux = str.substr(j, i);		// only string without whitespace
			if (aux.length())			// is not whitespace only
				v_exstr.push_back(aux);

			j += ++i; i = -1;			// moves j, resets i
		}
	}
	aux = str.substr(j, i);
	if (aux.length())
		v_exstr.push_back(aux);
}

/**
 * @brief Checks if a string is a general number, defined as:
 *	[-+]?[0123456789]+
 * @param s String to be checked
 * @return True if number else false
 */
static bool is_num_gen(const std::string &s)
{
	debug_info();
	std::string::const_iterator it = s.begin();
	bool first = true;
	while (it != s.end())
	{
		if (first)		// always runs at the first time
		{
			if (*it != '+' && *it != '-' && !(std::isdigit(*it)) )
				return false;

			first = false;
		}
		else 
		{
			if ( !std::isdigit(*it) )
				return false;
		}
		it++;
	}
	return !s.empty();
}

/**
 * @brief Checks if string has only numbers
 * @param s String to be checked
 * @return True if number else false
 */
static bool is_num_only(const std::string &s)
{
	debug_info();
	return !s.empty() && std::find_if(s.begin(), s.end(), 
		[](char c) { return !std::isdigit(c); }) == s.end();
}

/**
 * @brief Outputs starting symbol of scoreboard
 */
inline void start_symb() {
	std::cout << "SB> ";
}

/**
 * @brief "print" command, same as "scoreboard", "score", "show" 
 *	Prints actual scoreboard to the desired stream TODO
 */
void uc_print()
{
	debug_info();
	if (v_exstr.size() > 1)
		report_err("No such subcommand!", void());

	scb.print();		// TODO stream
}

/**
 * @brief "score" command processing, 
 * 	score	-> // shows scoreboard
 *			-> add 	-> (<name> | <rank>) [<number>]
 *			-> reset-> all | (<name> | <rank>)
 */
void uc_score()
{
	debug_info();
	switch(v_exstr.size())
	{
		case 1:					// "score"
			scb.print();
			return;
		case 3:				
			switch(m_cmd_parse[v_exstr[1]])
			{
				case SC_ADD:	// "score add (<name> | <rank>) 
					sc_add_sc();
					break;
				case SC_RESET:	// "score reset (all | (<name> | <rank>))"
					sc_reset();
					break;
				default:
					report_err("Unknown subcommand", void());
			}
			break;
		case 4:			
			if (m_cmd_parse[v_exstr[1]] == SC_ADD)
			{
				sc_add_sc();	// "score add (<name> | <rank>) <number>"
				return;
			}
		default:
			report_err("Unknown subcommand", void());
	}
}

/**
 * @brief Subcommand "add" of "score" command
 *  "score add (<name> | <rank>)"
 *  "score add (<name> | <rank>) <number>"
 */
void sc_add_sc()
{
	debug_info();
//	int num = 1;
	if (v_exstr.size() == 3)
	{
		if ( is_num_only(v_exstr[2]))	// checking rank correctness
			scb.add_pscore(std::stoi(v_exstr[2]));	// "score add <rank>"
		else
			scb.add_pscore(v_exstr[2]);				// "score add <name>"
	}
	else
	{
		if (is_num_gen(v_exstr[3]))	// "score add (<name>|<rank>)<number>"
		{
			if ( is_num_only(v_exstr[2]))	// checking rank correctness
				scb.add_pscore(std::stoi(v_exstr[2]), 
								std::stoi(v_exstr[3]));	// score add <rank>"
			else					// "score add <name>"
				scb.add_pscore(v_exstr[2], std::stoi(v_exstr[3]));	
				
		} else
			report_err("Wrong format of number", void());
	}
}

/**
 * @brief Subcommand "reset"
 *	score -> reset -> all
 *	score -> reset -> (<name>|<rank>)
 */
void sc_reset()
{
	debug_info();

	if (m_cmd_parse[v_exstr[2]] == SC_ALL)
		scb.reset_score();
	else
	{
		if (is_num_only(v_exstr[2]))		// is rank
			scb.reset_pscore(std::stoi(v_exstr[3]));
		else
			scb.reset_pscore(v_exstr[3]);	// is name
	}
}


/**
 * @brief Command "player" - modifies a player
 * 	player 	-> add [<name>] [<score>]
 *			-> remove -> all
 * 			-> rename -> (<name> | <rank>) <new_name>
 */
void uc_player()
{
	debug_info();

	switch(m_cmd_parse[v_exstr[1]])
	{
		case SC_ADD:
			sc_add_p();
			break;
		case SC_REMOVE:
			sc_remove();
			break;
		case SC_RENAME:
			sc_rename();
			break;
		default:
			report_err("Unknown subcommand", void());
	}
}


/**
 * @brief Subcommand "add" of "player" command
 * 	player -> add [<name>] [<score>]
 */
void sc_add_p()
{
	switch(v_exstr.size())
	{
		case 3:			// player add <name>
			scb.add_player(v_exstr[2]);
			break;
		case 4:			// player add <name> <score>
			if (is_num_gen(v_exstr[3]))
			{
				scb.add_player(v_exstr[2], std::stoi(v_exstr[3]));
				break;
			}
		default:
			report_err("Unknown subcommand", void());
	}
}

/**
 * @brief Subcommand "remove" of "player" command
 *	player -> remove -> all
 * 	player -> remove -> (<name> | <rank>)
 */
void sc_remove()
{
	switch(v_exstr.size())
	{
		case 3:			// player remove all
			if (m_cmd_parse[v_exstr[2]] == SC_ALL)
			{
				scb.rm_players();
				break;
			}
		case 4:			// player remove (<name> | <rank>)
			if (is_num_only(v_exstr[3]))
				scb.rm_player(std::stoi(v_exstr[3]));
			else
				scb.rm_player(v_exstr[3]);

			break;
		default:
			report_err("Unknown subcommand", void());
	}
}

/**
 * @brief Subcommand "rename" of "player" command
 * 	player -> rename -> (<name> | <rank>) <new_name>
 */
void sc_rename()
{
	if (v_exstr.size() != 4)
		report_err("Unknown subcommand", void());
	
	if (is_num_only(v_exstr[2]))
		scb.rename_player(std::stoi(v_exstr[2]), v_exstr[3]);
	else
		scb.rename_player(v_exstr[2], v_exstr[3]);
}

/**
 * @brief "win" command, adds a score of 1 to a player
 *	win -> <name> | <rank>
 */
void uc_win()
{
	debug_info();

	if (v_exstr.size() != 2)
		report_err("Unknown subcommand", void());

	if (is_num_only(v_exstr[2]))
		scb.add_pscore(std::stoi(v_exstr[1]));
	else
		scb.add_pscore(v_exstr[1]);
}

/**
 * @brief "loss" command, decrements a score of a player by one
 *	loss -> <name> | <rank>
 */
void uc_loss()
{
	debug_info();

	if (v_exstr.size() != 2)
		report_err("Unknown subcommand", void());

	if (is_num_only(v_exstr[2]))
		scb.add_pscore(std::stoi(v_exstr[1]), -1);
	else
		scb.add_pscore(v_exstr[1], -1);
}

/**
 * @brief "set" command, sets scoreboard variables
 *	set -> show <M>	- sets maximum number of shown players
 *	set -> max <N>	- sets maximum number of players
 */
void uc_set()
{
	debug_info();
	if (v_exstr.size() != 3)
		report_err("Unknown subcommand", void());

	switch(m_cmd_parse[v_exstr[1]])
	{
		case UC_SHOW:
			if (is_num_gen(v_exstr[2]))
			{
				scb.set_show_max(std::stoi(v_exstr[2]));
				break;
			}
		case SC_MAX:
			if (is_num_gen(v_exstr[2]))
			{
				scb.set_max_players(std::stoi(v_exstr[2]));
				break;
			}
		default:
			report_err("Unknown subcommand", void());
	}
}

/**
 * @brief TODO
 */
void uc_save()
{
	debug_info();
	
}

/**
 * @brief TODO
 */
void uc_load()
{
	debug_info();

}


/**
 * @brief Parses command line arguments using getopt
 */
void parse_args(int argc, char *argv[])
{
	char c;
	while ((c = getopt(argc, argv, "p:s:m:f:h")) != -1)
	{
		switch(c)
		{
			case 'p':
				std::cout << "p arg: " << optarg << std::endl;
				break;
			case 's':
				std::cout << "s arg: " << optarg << std::endl;
				break;
			case 'm':
				std::cout << "m arg: " << optarg << std::endl;
				break;
			case 'f':
				std::cout << "f arg: " << optarg << std::endl;
				break;
			case 'h':
				std::cout << "h arg: " << optarg << std::endl;
				break;
			default:
				std::cerr << "Error: Uknown command" << std::endl;
		}
	}
}

/**
 * @brief Main program
 */
int run_scb(int argc, char *argv[])
{
	// TODO arguments
	parse_args(argc, argv);
		
	// initializes scoreboard
//	Scoreboard scb();

	// initializes map with strings and codes
	m_cmd_parse = m_cmd_init();

	start_symb();					// prints the starting symbol if OK
	std::string user_in;
	while( std::getline (std::cin, user_in) )
	{
		split_str(user_in);					// vector of strings
		switch(m_cmd_parse[v_exstr[0]])		// with only main commands
		{
			case UC_PRINT:		
				uc_print();	
				break;
			case UC_SCOREBOARD:
				uc_print();
				break;
			case UC_SHOW:
				uc_print();
				break;
			case UC_SCORE:
				uc_score();
				break;
			case UC_PLAYER:
				uc_player();
				break;
			case UC_WIN:
				uc_win();
				break;
			case UC_LOSS:
				uc_loss();
				break;
			case UC_SET:
				uc_set();
				break;
			case UC_SAVE:
				uc_save();
				break;
			case UC_LOAD:
				uc_load();
				break;
			case UC_HELP:
				std::cout << help_cmdl << std::endl;
				break;
			case UC_EXIT:
				return EXIT_SUCCESS;
			default:
				debug_msg("default");
				std::cerr << "Error: No known command" << std::endl;
		}
		start_symb();
	}

	return EXIT_SUCCESS;	// TODO
}
