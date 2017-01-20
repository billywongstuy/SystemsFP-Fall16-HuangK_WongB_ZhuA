# Big Two aka Chinese Poker

## Members
Karen Huang, Billy Wong, Alvin Zhu


## Setup

1. Have 5 systems ready to run.
2. Compile all the files on all systems by typing into terminal: ```make```
3. On one system, type into terminal: ```./server```. Take note of the IP address of that system.
4. On the other 4 systems, type into terminal: ```./client <IP Address>``` , where \<IP Address\> is the IP address of the server.


## How to Use




## List of Files That SHOULD be in the Directory

cards.c
cards.h
client.c
client.h
DESIGN.txt
devlog.txt
input.c
input.h
kill_ipcs.sh
makefile
networking.c
networking.h
players.c
players.h
printMethods.c
printMethods.h
README.md
rules.c
rules.h
server.c
server.h
turns.c
turns.h

<b>Other files are not vital to the program</b>


## Notes

(Before we implement signals)
* Be sure to exit all clients before exiting the server.
  * Failure to comply will result in a server attachment error that will last for a short while.
