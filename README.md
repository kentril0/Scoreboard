# Scoreboard

A terminal like app, that shows a table with ranking, player names and player scores. Used for LAN parties or any competitions up to 65535 players.

Requirements
TODO
g++ version 7

Installation
TODO

Commandline arguments
Usage: ./scoreboard [-p P] [-s S] [-m M] [-sf file] [-hf histFile] [-h] [--help]
Options:
 -p P		Initializes scoreboard P players, where P is number of players,
 			max being a set limit of players.
 -s S		Sets S players that will be shown when score table is print.
 -m M		Sets maximum number of players (Player limit).
 -sf file 	Sets a file path to a save file, scoreboard will now print to 
 			a file instead of STDOUT.
 -hf file	Sets a path to a history file with printed scoreboard, data
 			will load into current scoreboard.
 -h/--help	Shows this message.

Notes:
 If both arguments "-p" and "-hf" are used and are valid, first players
 are initialized and after then the history file is loaded, but players
 are added only up to the current available limit.

Scoreboard Commands
// TODO sync with real 
print | scoreboard | show | score	- shows current score table
player 	-> add [<name>] [<score>]
		-> remove -> all | (<name> | <rank>)
		-> rename -> (<name> | <rank>) <new_name>
score	-> add 	-> (<name> | <rank>) [<number>]
		-> reset -> all | (<name> | <rank>)
win		-> <name> | <rank>
loss	-> <name> | <rank>
set		-> show <M>
		-> plimit <N>
		-> file <path_to_file>
save	// nothing if file specified
		-> <path_to_file>
		-> history -> <path_to_file>
load	-> history -> <path_to_file>
		-> players -> <path_to_file>
help	- shows this message
exit	- shuts down the scoreboard app


Comments

FIRST PROTOTYPE THEN OPTIMITZE
- string to char*
- remove Player class and move it to Scoreboard class
- debug messages to each Scoreboard function

Maximum players shown
MAX_INT

Maximum players
MAX_INT
soft limit - 255, can be changed
- When setting a limit players above the limit will be deleted automatically

Player Names
^([a-z]|[A-Z]|[0-9]|[_@$*-])*$
max - 32 chars + possible chars (N), N up to 65534 and \0 -> 8
(1)Player -> 40 chars hard limit
		 11111111112222222222333
12345678901234567890123456789012
if no name provided, a name will be generated automatically in a format:
	"Player_%d", N
- is unique, if name already there, (N) is prepended, where N is 


TODO exceptions
TODO 
sizeof class Player vs sizeof struct Player
unordered map -> presypat do  vec

Unordered map -> key = char *, int score	- uniqueness, access
vector -> key = int score, char *			- sorting, rank access

Player - struct, pointers
safety later - like const poiknters atd.
no repetition of code -> make macros or functions rather
todo includes
