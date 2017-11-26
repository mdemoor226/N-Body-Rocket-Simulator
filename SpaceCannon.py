#! /usr/bin/python

#SpaceCannon.py 
#example to run: ./SpaceCannon.py
import numpy 
import sys 
import os
from subprocess import call
import subprocess

print "**** Welcome to the Space Cannon Ballistics Simulation ****"
print ""

#User needs to input all possible Launch angles (Theta and PHI) they would like to test, along with how to divide up the possible Launch Angles (Remember the Grids we were talking about).
print "Enter intial launch position: "
launchposition = raw_input()

#User needs to input range of all possible wait times they would like to test, along with how to divide up the wait times.
print "Enter maximum length of time to wait before launching: "
launchwaittime = input()

#This is fine
print "Enter time willing to wait for object to travel: "
traveltime = input()

'''
print ""
print "You have entered the following parameters:"
print "Initial Launch Position: ", launchposition
print "Maximum Length of Time to Wait Before Launching: ", launchwaittime
print "Time Willing to Wait for Object to Travel: ", traveltime
print "----------------------------------------------"

print ""
'''

# Run the C++ Space simulation executable file, named 'Space'	
os.system("EE5243-Project/Space")

# This code reads in values from the Simfile, and then prints the data.//I'm not sure what simfile is//I think this should be writing the values received above to a file for the master
#node in 'MPIprocessfile.py' to interpret and then scatter instead//
data = []
file = open("Simfile", "r")
for line in file:
	data.append(line)

print data
file.close()

#This code calls the mpiexec command to spawn the MPIprocessfile.py file 2 times.//This will have to be an MPI Run command in the end, but MPI Exec is good for now//
command = "mpiexec -n 2 python ./MPIprocessfile.py"
os.system(command)
print ""








