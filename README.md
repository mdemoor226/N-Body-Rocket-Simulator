This is a CLI N-Body Simulator capable of gravitaionally simulating any number of celestial objects over a specified time span. In addition to gravitaionally simulating planets, stars, moons, and anything else it is capable of simulating a rocket launch given initial conditionals and launching a mass swarm of these simulations in parrallel within an HPC environment.  

To Run The Program:
Step 1: chmod 533 run.sh 
Step 2: ./run.sh
One of the C++ programs will generate some unimportant warnings while compiling. Just ignore them.

Needs to be compiled and run within a Linux environment. Uses openmpi to grid-search optimal rocket launch parameters.
