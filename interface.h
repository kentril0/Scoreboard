/**
 * @file interface.h
 * @date 29.08.2018
 * @author Kentril Despair
 * @brief User interaction header file
 * TODO
 */

#ifndef INTERFACE_H
#define INTERFACE_H

/**
 * @brief Numeric constants for user commands
 */
enum user_cmnds
{
	// user commands
	UC_PRINT = 550,
	UC_SCOREBOARD,
	UC_SHOW,
	UC_SCORE,
	UC_PLAYER,
	UC_WIN,
	UC_LOSS,
	UC_SET,
	UC_SAVE,
	UC_LOAD,
	UC_HELP,
	UC_EXIT,

	// subcommands
	SC_ADD,
	SC_REMOVE,
	SC_RENAME,
	SC_RESET,
	SC_MAX,
	SC_FILE,
	SC_HISTORY,
	SC_PLAYERS,
	SC_ALL
};

// help message usage
const char *const help_usg =
 "Usage: ./scoreboard [-p P] [-s S] [-m M] [-sf file] [-hf histFile] [-h] "
 "[--help]\n"
 "Options: \n"
 " -p P      Initialzes scoreboard with P players, where P is the number\n"
 "           of players, max being a set limit of players\n"
 " -s S      Sets S players that will be shown when score table is print\n"
 " -m M      Sets maximum number of players (Player limit)\n"
 " -sf file  Sets a file path to a save file, scoreboard will now print \n"
 "           to a file instead of STDOUT\n"
 " -hf file  Sets a path to a history file with printed scoreboard, data\n"
 "           will load into the current scoreboard\n"
 " -h|--help Shows this message.\n";

// help message - commands
const char *const help_cmds = 
 "print | scoreboard | show | score\t- show current score table\n"
 "player\t-> add [<name>] [<score>]\n"
 "\t-> remove ( all | ( <name> | <rank> ) )\n"
 "\t-> rename (<name> | <rank>) <new_name>\n"
 "score\t-> add (<name> | <rank>) [<number>]\n"
 "\t-> reset ( all  (<name> | <rank>) )\n"
 "win\t-> <name> | <rank>\n"
 "loss\t-> <name> | <rank>\n"
 "set\t-> show <SHOW_PLAYERS>\n"
 "\t-> plimit <MAX_PLAYERS>\n"
 "\t-> file <path_to_file_for_saving>\n"
 "save\t-> // nothing if save file path specified\n"
 "\t-> file <path_to_file_to_save>\n"
 "\t-> history <path_to_save_history_file>\n"
 "load\t-> history <path_to_history_file>\n"
 "\t-> players <path_to_players_name_file>\n"
 "help\t- show this message\n"
 "exit\t- shuts down the scoreboard app\n";

/**
 * @brief Parsed commandline args are saved here
 */
struct Args
{
	int init_plrs;	///< Number of initial players
	int max_show;	///< Number of maximum shown players
	int max_plrs;	///< Player limit
	char *sf_path;	///< Path to a save file
	char *hf_path;	///< Path to a history file
};

int run_scb(int argc, char *argv[]);
void parse_args(int argc, char *argv[]);

inline void start_symb();

// user commands
void uc_print();
void uc_score();
void uc_player();
void uc_win();
void uc_loss();
void uc_set();
void uc_save();
void uc_load();

// user subcommands
void sc_add_sc();
void sc_add_scn();
void sc_reset();
void sc_add_p();
void sc_remove();
void sc_rename();


#endif
