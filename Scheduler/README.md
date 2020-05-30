Say that you are an engineer trying to assign machines to your newly created distributed system
consisting of a large number of components. You have thousands of machines at your disposal
in data centers around the world. The catch is that most, if not all, machines are already running
some processes. The input to your program, coming from our Inventory Management System, is
a list of machines together with their loaded factor (from 1-10) and their location as well as a
matrix of distances between various data centers. 

Scheduler automatically suggests a subset of machines out of the available pool (machines with the loaded factor < 10), so that their collective network distance is minimal.

Loaded factor is defined as the number of components running on a machine; that is, a machine
with a loaded factor of 10 cannot sustain any more components, whereas a machine with a
loaded factor of 4 can sustain 6 more components.

