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
#include <unordered_map>


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

inline void start_symb();


void uc_print();

void uc_score();

#endif
