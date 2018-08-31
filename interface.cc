/**
 * @file interface.cc
 * @date 31.08.2018
 * @author Kentril Despair
 * @brief
 *
 */

#include "interface.h"
#include "scoreboard.h"
#include <unistd.h>

static std::vector<std::string> v_exstr;
static std::unordered_map<std::string, user_cmnds> m_cmd_parse;
static Scoreboard scb;

/**
 * @brief Initializes map 
 */
static std::unordered_map<std::string, user_cmnds> &&m_cmd_init()
{
	// initialized map with appropriate keywords - commands and their
	// codes
	std::map<std::string, user_commands> m_aux ( 
		{{"print", UC_PRINT}, {"scoreboard", UC_SCOREBOARD}, 
		{"show", UC_SHOW}, {"score", UC_SCORE}, {"player", UC_PLAYER}, 
		{"win", UC_WIN}, {"loss", UC_LOSS}, {"set", UC_SET}, 
		{"save", UC_SAVE}, {"load", UC_LOAD}, {"add", SC_ADD}, 
		{"remove", SC_REMOVE}, {"rename", SC_RENAME}, {"reset", SC_RESET}, 
		{"max", SC_MAX}, {"file", SC_FILE}, {"history", SC_HISTORY}, 
		{"players", SC_PLAYERS}, {"all", SC_ALL}}
	);

	return &&m_aux;
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
 * @brief
 */
void uc_score()
{
	debug_info();

}

/**
 * @brief
 */
void uc_player()
{
	debug_info();

}

/**
 * @brief
 */
void uc_win()
{
	debug_info();

}

/**
 * @brief
 */
void uc_loss()
{
	debug_info();

}

/**
 * @brief
 */
void uc_set()
{
	debug_info();

}

/**
 * @brief
 */
void uc_save()
{
	debug_info();

}

/**
 * @brief
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

	start_symb();					// prints the starting symbol if OK
	std::string user_in;
	while( std::getline (std::cin, user_in) )
	{
		explode_str(user_in);		// vector of strings
		switch(m_cmd_parse[exp_str[0]])		// with only main commands
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
			default:
				debug_msg("default");
				std::cerr << "Error: No known command" << std::endl;
		}
		start_symb();
	}

	return EXIT_SUCCESS;	// TODO
}
