ASSUMPTION:
-----------
1. Modify always only alters qty, reject price mods for now.
2. A single tick corresponds to one of the top bid/ask level of the book.
3. output is to be sent to stdout.

ANALYSIS:
---------
Book is represented as an array of Levels for each side. Each level maintains the order list, total qty and total px. For our use case of printing mid, this is efficient. Maintaining an order list for the level naturally renders itself to algorithms for handling of ticks.

Parsing of message currently relies on tokenization of string. It also relies on exception handling of std::stol and std::stoi to identify incorrect format message. This can be improved in future versions. 

PERFORMANCE:
-------------
$ time ./feedhandler perfInput.txt > /dev/null

real  0m7.622s
user  0m6.975s
sys 0m0.592s

It takes around 7 seconds to run the program for a million entries on my 2.3 GHz intel i5 macbook pro.

RUN INSTRUCTIONS:
-----------------
make clean;
make;
./feedhandler ./input/view.txt

3 input files:
./input/errorInput.txt - to view error handling
./input/viewResult.txt - to view the output according to specs
./input/perfInput.txt - contains a million entries. The results are posted above
