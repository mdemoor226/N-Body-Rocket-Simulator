#! /usr/bin/python

import sys 
import os
from subprocess import call
import subprocess

Points = []#List of Lists containing Launch angles
Grid = [] #List of Lists containing Grid points

def VerifyPointInput():
	while(True):
		IN = raw_input()
		if(IN.lower() == "done"):
			return "done"
		try:
			theta =  float(IN)
			return theta
		except ValueError:
			print "You messed up. Try again."

def VerifyInput():
	while(True):
		IN = raw_input()
		try:
			IN =  float(IN)
			return IN
		except ValueError:
			print "You messed up. Try again."

def VerifyTimeInput():
	while(True):
		Out = VerifyInput()
		if(0 <= Out):
			return Out
		print "Error, Time values must be nonnegative"

def VerifyResInput():
	while(True):
		Out = VerifyInput()
		if(0 < int(Out)):
			return int(Out)
		print "Error, Resolution Inputs must be strictly positive"


def Point(TimeRes, waittime, traveltime):
	count = 1
	print "Specify Theta and Phi angles for each Launch Point. Enter 'done' when you are finished."
	#Specify Possible Launch Points
	while(True):
		print "Launch Position Number: ", count

		#Specify Launch Angle Theta
		print "Enter Launch angle Theta:"
		theta = VerifyPointInput()
		if(str(theta) == "done"):
			break
		print "THETA is ", theta 
		
		#Specify Launch Angle Phi		
		print "Enter Launch Angle Phi:"
		phi = VerifyPointInput()
		if(str(phi) == "done"):
			break
		print "PHI is ", phi 		
		
		theta = theta % 360
		phi = phi % 360
		
		#Append point along with all TIME and WAIT variables
		CountTime = 0		
		while(CountTime <= TimeRes):
			Point = []
			Wtime = CountTime*(waittime/TimeRes)
			Point.append(theta)
			Point.append(phi)			
			Point.append(Wtime)
			Point.append(int(traveltime))
			Points.append(Point)
			CountTime = CountTime+1		
		count = count+1	

def Gridier(TimeRes, waittime, traveltime):
	#Enter info for Theta
	while(True):
		print "Please enter angle resolution for launch angle Theta."
		Mt = VerifyResInput()

		#Enter Min Theta				
		print "Please enter minimum value for launch angle Theta."
		MinT = VerifyInput()
		MinT = MinT % 360
	
		#Enter Max Theta				
		print "Please enter maximum value for launch angle Theta."
		MaxT = VerifyInput()
		MaxT = MaxT % 360

		RangeT = MaxT - MinT
		if(MinT <= MaxT):
			break
		print "Error, min must be less than max."

	#Enter info for Phi
	while(True):
		print "Please enter angle resolution for launch angle Phi."
		Mp = VerifyResInput()

		#Enter Min Phi				
		print "Please enter minimum value for launch angle Phi."
		MinP = VerifyInput()
		MinP = MinP % 360
	
		#Enter Max Phi				
		print "Please enter maximum value for launch angle Phi."
		MaxP = VerifyInput()
		MaxP = MaxP % 360

		RangeP = MaxP - MinP
		if(MinP <= MaxP):
			break
		print "Error, min must be less than max."
	
	#Append all possible points to grid	
	CountTime = 0
	while(CountTime <= TimeRes):
		CountP = 0
		Wtime = CountTime*(waittime/TimeRes)
		while (CountP <= Mp):
			CountT = 0
			Phi = CountP*(RangeP/Mp) + MinP
			while (CountT <= Mt):
				gridPoint = []	
				Theta = CountT*(RangeT/Mt) + MinT
			
				gridPoint.append(Theta)			
				gridPoint.append(Phi) 

				#Append TIME variable and WAIT variable
				gridPoint.append(Wtime)
				gridPoint.append(int(traveltime))

		
				Grid.append(gridPoint)
				CountT = CountT+1
			CountP = CountP+1
		CountTime = CountTime + 1

print "**** Welcome to the Space Cannon Ballistics Simulation ****\n"

#User needs to input the TIME they are willing to wait till Rocket Launch. The total calculation will be split up into possible time periods in seconds.
print "Enter the maximum TIME you are willing to wait till Rocket Launch. \nThis will be entered as how many Days, Hours, and Minutes:"
print "How many DAYS?"	
days = VerifyTimeInput()

print "How many HOURS?"
hours = VerifyTimeInput()

print "How many MINUTES?"
minutes = VerifyTimeInput()

waittime = (int(days)*86400) + (int(hours)*3600) + (int(minutes)*60)

print "Now specify the resolution you would like to use to divide up the possible launch times."
TimeRes = VerifyResInput()

###############################################################################################################################################
#User needs to input the time to WAIT for having the rocket hit the object (the total calculation will be in seconds).
print "\nEnter time willing to WAIT for object to travel.  \nThis will be entered as how many Days, Hours, and Minutes:"
print "How many DAYS?"
days = VerifyTimeInput()

print "How many HOURS?"
hours = VerifyTimeInput()

print "How many MINUTES?"
minutes = VerifyTimeInput()

traveltime = (int(days)*86400) + (int(hours)*3600) + (int(minutes)*60)

print "travel time is", traveltime
print "waittime is ", waittime

#########################################Receive Configuration Instruction from User########################################################
print "\nWould you like to specify a grid of possible launch positions on the launch planet,  or a specified number of points? Enter 'Grid' or 'Points'."
while(True):
	IN = raw_input()
	IN = IN.lower()

	if(IN == "grid" or IN == "points"):
		break
	print "Error, please enter either 'Grid' or 'Points'"
############################################################################################################################################

if(IN == "grid"):
	Gridier(TimeRes, waittime, traveltime)
	#Write Possible Launch Points and Wait Times to a file
	file = open("file.conf", "w")
	for item in Grid:
		file.write("%s\n" % item)
	file.close()
else:
	Point(TimeRes, waittime, traveltime)
	#Write Possible Launch Points and Wait Times to a file
	file = open("file.conf", "w")
	for item in Points:
		file.write("%s\n" % item)
	file.close()

############################################################################################################################################

print "\n**** Simulation ****"

# Run the C++ Space simulation executable file, named 'Space'	
os.system("./Space")

#Call MPIEXEC/RUN
#command = "mpiexec -n " + str(len(Grid)) + " python ./MPIprocessfile.py" if(len(Points) == 0) else "mpiexec -n " + str(len(Points)) + " python ./MPIprocessfile.py"
command = "mpirun -np " + str(len(Grid)) + " -machinefile host.txt python ./MPIprocessfile.py" if(len(Points) == 0) else "mpirun -np " + str(len(Points)) + " -machinefile host.txt python ./MPIprocessfile.py"#Need to specify Hostnames
os.system(command)


