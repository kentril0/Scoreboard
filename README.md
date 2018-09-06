# Scoreboard

A text application for Unix based systems, that shows a table with ranking, 
player names and player scores - a scoreboard. 
Used for LAN parties or any competitions.
Just a fun little application, one of my first C++ apps.  

## Example of output 

```
________________________________________________________________________  
 | RANK   | PLAYER NAME                                          | SCORE  |  
  ------------------------------------------------------------------------  
 | 1.     | Valmir                                               | 50     |   ------------------------------------------------------------------------
 | 2.     | Shade                                                | 26     |
  ------------------------------------------------------------------------
 | 3.     | Kentril                                              | 25     |
  ------------------------------------------------------------------------
 | 4.     | Wormik                                               | 23     |
  ------------------------------------------------------------------------

```

## Status
 **File manipulation is not implemented yet.**  
 **Regular expression comparison for player names not implemented yet.**

## Requirements
* g++ min. version 7 (C++17)
* Makefile

## Installation
Extract into a dir and "install" using make command

## Commandline Usage

Shown when "./scoreboard --help | -h" used:  

./scoreboard [-p P] [-s S] [-m M] [-sf file] [-hf histFile] [-h] [--help]  
Options:  
 -p P		Initializes scoreboard with P players, where P is the number of   
 			players, max being a set limit of players.  
 -s S		Sets S players that will be shown when score table is print.  
 -m M		Sets maximum number of players (Player limit).  
 -sf file 	Sets a file path to a save file, scoreboard will now print to  
 			a file instead of STDOUT.  
 -hf file	Sets a path to a history file with printed scoreboard, data  
 			will load into the current scoreboard.  
 -h|--help	Shows this message.  

### Notes:
 If both arguments "-p" and "-hf" are used and are valid, first players  
 are initialized and after then the history file is loaded, but players  
 are added only up to the current available limit.  

## Scoreboard Commands

print | scoreboard | show | score	- shows current score table  
player 	-> add [<name>] [<score>]  
		-> remove ( all | (<name> | <rank>) )  
		-> rename (<name> | <rank>) <new_name>  
score	-> add (<name> | <rank>) [<number>]  
		-> reset ( all | (<name> | <rank>) )  
win		-> <name> | <rank>  
loss	-> <name> | <rank>  
set		-> show <SHOW_PLAYERS>  
		-> plimit <MAX_PLAYERS>  
		-> file <path_to_file_for_saving>  
save	-> // nothing if file specified  
		-> <path_to_file_to_save>  
		-> history <path_to_save_history_file>  
load	-> history <path_to_history_file>  
		-> players <path_to_player_name_file>  
help	- shows this message  
exit	- shuts down the scoreboard app  


## Limits
1. Number of players and limit
	- by default 0 players are created
	- by default soft limit of players is 255
	- by default hard limit of players is 65535 (unsigned short)
	- players can be initialized with default names and scores (Player, 0)
		with commandline option "-p P" up to the set limit
	- limit can be set using commandline option "-m M" or using 
		command "set plimit <MAX_PLAYERS>"

2. Number of shown players
	- affects output of scoreboard, shows only first N players, where N
		is the limit of players that can be shown
	- by default shows all players
	- can be set only up to the hard limit of players

3. Player name
	- maximum length of 32 characters
	- regular expression: ^([a-z]|[A-Z]|[0-9]|[_@$*-])*$
	- if no name provided, player with a default name is created

4. Player rank
	- player can be referenced by name or by rank - position in the 
		scoreboard
	- can be only positive number

4. Player score
	- by default is set to 0
	- can be in range from -9999 to 9999 


## Comments

Maximum players  
soft limit - 255, can be changed  
- When setting a limit players above the limit will be deleted automatically

Player Names
max - 32 chars + possible chars (N), N up to 65534 and \0 -> 8  
Player(1) -> 40 chars hard limit  
		 11111111112222222222333  
12345678901234567890123456789012  
if no name provided, a name will be generated automatically in a format:  
	"Player(%d)", N  
- is unique, if name already there, (N) is appended  

### TODO 
TODO exceptions  
no repetition of code -> make macros or functions rather  
todo includes  
