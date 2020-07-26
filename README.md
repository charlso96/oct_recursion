# Source code for simulating an example Octahedron Relation System

## Keonwoo Oh Swarthmore College 2020

Refer to the pdf for more detail about the system.

Follow the following instructions to compute and plot recursion.

Computation
------------
Computation is implemented using C:

* make to compile the code

* ./main -k k_value -s sigma -t tau -b boardsize -i inputfile -o outputfile -h help

k_value is the last k_value of the recursion. Sigma and tau are both initial conditions.
Both values can be in form of fraction such as "1/2" or integer as the code parses it accordingly.
Input file has coordinates of initial condition (i,j,k) followed by initial condition at the corresponding coordinates.
The format is as follows:

number of coordinates

i j k initial condition

For example,

1

0 0 1 1

denotes an initial condition of one coordinate of (0,0,1) whose initial condition value is one. Initial condition could also be either fraction or integer as it is parsed accordingly.  
The results of the computation is a set of (i/k,j/k,rho) coordinates for the given k value.
The program writes the coordinates into the output file as a list of coordinates.
The program is not parallelized and hence, for large k value, it may take a few minutes. Do not quit in the middle as it may lead to memory leak.

Plotting
-------------------
TBD
