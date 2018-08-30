/**
 * @file
 *
 */

#include "scoreboard.h"
#include "interface.h"
#include <unordered_map>

int main()
{
	// initialized map with appropriate keywords - commands and their
	// codes
	std::map<std::string, user_commands> m_cmd_parse ( 
		{{"print", UC_PRINT}, {"scoreboard", UC_SCOREBOARD}, 
		{"show", UC_SHOW}, {"score", UC_SCORE}, {"player", UC_PLAYER}, 
		{"win", UC_WIN}, {"loss", UC_LOSS}, {"set", UC_SET}, 
		{"save", UC_SAVE}, {"load", UC_LOAD}, {"add", SC_ADD}, 
		{"remove", SC_REMOVE}, {"rename", SC_RENAME}, {"reset", SC_RESET}, 
		{"max", SC_MAX}, {"file", SC_FILE}, {"history", SC_HISTORY}, 
		{"players", SC_PLAYERS}, {"all", SC_ALL}}
	);

	// TODO arguments
		
//	Scoreboard scb();

	start_symb();					// prints the starting symbol if OK

	std::string user_in;
	while( std::cin >> user_in )
	{
		switch(m_cmd_parse[user_in])
		{
			case UC_PRINT:
				debug_msg("print");
				
				break;
			case UC_SCOREBOARD:
				debug_msg("scoreboard");

				break;
			case UC_SHOW:
				debug_msg("show");

				break;
			case UC_SCORE:
				debug_msg("score");

				break;

			case UC_PLAYER:
				debug_msg("player");

				break;

			case UC_WIN:
				debug_msg("win");

				break;

			case UC_LOSS:
				debug_msg("loss");

				break;

			case UC_SET:
				debug_msg("set");

				break;

			case UC_SAVE:
				debug_msg("save");

				break;

			case UC_LOAD:
				debug_msg("load");

				break;

			default:
				debug_msg("default");

		}
		start_symb();
	}

	return EXIT_SUCCESS;
}
