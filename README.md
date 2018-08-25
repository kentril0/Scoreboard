# Scoreboard
A terminal like app, shows a table with ranking, players and player scores, e. g. for LAN parties

Installation
TODO

Commandline arguments
TODO 

Commands
TODO

Comments

Maximum players shown
MAX_INT

Maximum players
MAX_INT
soft limit - 255, can be changed
- When setting a limit players above the limit will be deleted automatically

Player Names
^([a-z]|[A-Z]|[0-9]|[_@$*-])*$
max - 255 chars
if no name provided, a name will be generated automatically in a format:
	"Player_%d", N
- does not have to unique
