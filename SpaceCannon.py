#! /usr/bin/python

#SpaceCannon.py 
#example to run: ./SpaceCannon.py
import numpy 
import sys 
import os
from subprocess import call
import subprocess



print "**** Welcome to the Space Cannon Ballistics Simulation ****\n"

#User needs to input all possible Launch angles (Theta and PHI) they would like to test, along with how to divide up the possible Launch Angles (Remember the Grids we were talking about).
print "Enter intial launch position: "
launchposition = raw_input()

#User needs to input range of all possible wait times they would like to test, along with how to divide up the wait times.
print "Enter maximum length of time to wait before launching: "
launchwaittime = input()

#This is fine
print "Enter time willing to wait for object to travel: "
traveltime = input()


#########################################Receive Configuration Instruction from User########################################################
print "Would you like to specify a grid of possible launch positions on the launch planet,  or a specified number of points? Enter 'Grid' or 'Points'."
while(True):
	IN = raw_input()
	IN = IN.lower()

	if(IN == "grid" or IN == "points"):
		break
	print "Error, please enter either 'Grid' or 'Points'"
############################################################################################################################################
Points = []#List of Lists containing Launch angles
if(IN == "grid"):
	#Ask user to specify grid resolution
	print ""
	#Create a loop to append Points with Lists of Launch angles determined by Grid Resolution 
	
else: 
	count = 1
	print "Specify Theta and Phi angles for each Launch Point. Enter 'done' when you are finished."
	#Specify Possible Launch Points
	while(True):
		Point = []
		print "Launch Position Number: ", count
		#Specify Launch Angle Theta
		print "Enter Launch angle Theta"
		IN = raw_input()
		if(IN.lower() == "done"):
			break

		#Need to test whether user inputted a floating point value
		theta =  float(IN)
		Point.append(theta)
		print "THETA is ", theta
		
		#Specify Launch Angle Phi
		print "Enter Launch Angle Phi:"
		IN = raw_input()
		if(IN.lower() == "done"):
			break

		#Again, need to change code later to test whether or not user inputted a floating point number
		phi = float(IN)
		Point.append(phi)
		print "PHI is ", phi

	
		Points.append(Point)
		count = count+1
############################################################################################################################################

#Write Possible Launch Points and Wait Times to a file
#####################Make Necessary Changes###################
data = []
file = open("Simfile.txt", "r")
for line in file:
	data.append(line)

print data
file.close()
##############################################################



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

#This code calls the mpiexec command to spawn the MPIprocessfile.py file 2 times.//This will have to be an MPI Run command in the end, but MPI Exec is good for now//Might be alot more than 2 in the end
command = "mpiexec -n 2 python ./MPIprocessfile.py"
os.system(command)
print ""








