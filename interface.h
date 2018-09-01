/**
 * @file interface.h
 * @date 29.08.2018
 * @author Kentril Despair
 * @brief User interaction header file
 * TODO
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <cctype>

/**
 * @brief Numeric constants for user commands
 */
enum user_cmnds
{
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
void sc_reset();
void sc_add_p();
void sc_remove();
void sc_rename();


#endif
