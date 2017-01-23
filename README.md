# Big Two aka Chinese Poker

## Members
Karen Huang, Billy Wong, Alvin Zhu


## Setup

1. Have 5 systems ready to run.
2. Compile all the files on all systems by typing into terminal: ```make```
3. On one system, type into terminal: ```./server```. Take note of the IP address of that system.
4. On the other 4 systems, type into terminal: ```./client <IP Address>``` , where \<IP Address\> is the IP address of the server.


## How to Use

1. Choose your cards by inputting the number below the card(s) you wish to pick.
2. Input "p" if you want to pass. If 3 people pass in the row, you get a freebie! Throw out any valid hand. 
3. Press enter to check if it is your turn. It will tell you on the top. The turn order is 1->2->3->4->1->etc. The first player has the 3 of Diamond(you must start with it).
4. If you don't know basic rules, Google is your friend!!!
5. No butterfly/two pairs or dragon hands. Four of a kind must be matched with a single!

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
<b>*Be sure to exit all clients before exiting the server.
  * Failure to comply will result in a server attachment error that will last for a short while.</b>

## Bugs/Unfinished Features
1. Blocking not done properly so we compromised with the "Not your turn!!" statments
2. Networking sometimes inconsistent so sometimes wouldn't pass the entire updated buffer