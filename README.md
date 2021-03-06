# Big Two aka Chinese Poker

## Members
Karen Huang, Billy Wong, Alvin Zhu


## Setup

1. Have 5 system terminals ready to go (cd into correct directory!).
2. Compile all the files on all systems by typing into terminal by typing in: ```make```
3. On one terminal, type in: ```./server```. 
4. Take note of the IP address of that system.
5. On the other 4 terminals, type in: ```./client <IP Address>``` , where \<IP Address\> is the IP address of the server. If you are running the game all on one computer, just do: ```./client```


## How to Use

1. Choose your cards by inputting the number below the card(s) you wish to pick.
2. Whoever has the 3 of Diamond has the first move, though you must start with it.
3. Press enter to refresh the user interface and see if it's your turn. You will know when it is your turn if it says "Invalid selection(s)" on the top. The turn order is 1->2->3->4->1->etc, based on whoever is the first player. NOTE: if anything screws up, repeatedly press enter until it looks good again. Also, make sure not to hold enter.
4. To pick your cards, enter the corresponding number below each card. Separate your cards chosen with spaces or commas. Ex: 1,2 for your first 2 cards. OTHERWISE, to pass: 
5. Input "p" if you want to pass. If 3 people pass in a row, you get a freebie! Throw out any valid hand for the freebie move. 
6. If you don't know basic rules, Google is your friend!!! NOTE: No butterfly/two pairs or dragon hands. Four of a kind must be matched with a single!
7. When one player has no cards left, they win!

## List of Files That SHOULD be in the Directory

* cards.c
* cards.h
* client.c
* client.h
* DESIGN.txt
* devlog.txt
* input.c
* input.h
* kill_ipcs.sh
* makefile
* networking.c
* networking.h
* players.c
* players.h
* printMethods.c
* printMethods.h
* README.md
* rules.c
* rules.h
* server.c
* server.h
* turns.c
* turns.h

<b>Other files are not vital to the program</b>


## Notes

* Be sure to exit all clients before exiting the server.
* Failure to comply will result in a server attachment error that will last for a short while.
  * Would be fixed if signals were implemented

## Bugs/Unfinished Features
1. Blocking not done properly so we compromised with the "Not your turn!!" statments
2. Networking sometimes inconsistent so sometimes wouldn't pass the entire updated buffer
3. Location of "Your Info" and "Choice Result" swapped if server and client are on the same IP address (or if the network passing screws up).

## Screenshot

UI should look something along the lines of this (at least if using one system for all 4 players).
https://drive.google.com/file/d/0B1jCTECz-7BDRm1OZ2xoeldDTDA/view?usp=sharing
