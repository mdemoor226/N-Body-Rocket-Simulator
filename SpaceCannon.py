#! /usr/bin/python

#SpaceCannon.py 
#example to run: ./SpaceCannon.py
import numpy 
import sys 
import os
from subprocess import call
import subprocess


Points = []#List of Lists containing Launch angles
Grid = [] #List of Lists containing Grid points

#def VerifyInput():
#	pass

def Point(TimeRes, waittime, traveltime):
	count = 1
	print "Specify Theta and Phi angles for each Launch Point. Enter 'done' when you are finished."
	#Specify Possible Launch Points
	while(True):
		Point = []
		print "Launch Position Number: ", count

		#Specify Launch Angle Theta
		print "Enter Launch angle Theta:"
		while(True):
			IN = raw_input()
			if(IN.lower() == "done"):
				return
			try:
				theta =  float(IN)
				break
			except ValueError:
				print "You messed up. Try again idiot."
		print "THETA is ", theta 
		
		#Specify Launch Angle Phi		
		print "Enter Launch Angle Phi:"
		while(True):
			IN = raw_input()
			if(IN.lower() == "done"):
				return
			try:
				phi =  float(IN)
				break
			except ValueError:
				print "You messed up. Try again idiot."
		print "PHI is ", phi 		
		
		theta = theta % 360
		phi = phi % 360
		#Append TIME variable and WAIT variable
		CountTime = 0		
		while(CountTime <= TimeRes):
			Wtime = CountTime*(waittime/TimeRes)
			Point.append(theta)
			Point.append(phi)			
			Point.append(Wtime)
			Point.append(int(traveltime))
			CountTime = CountTime+1

		Points.append(Point)
		count = count+1	

	#Write Possible Launch Points and Wait Times to a file
	##############################################################
	file = open("file.conf", "w")
	for item in Grid:
		file.write("%s\n" % item)
	file.close()
	##############################################################

def Grid(TimeRes, waittime, traveltime):
	#Enter info for Theta
	print "Please enter angle resolution for launch angle Theta."
	while(True):
		Mt = raw_input()
		try:
			Mt =  int(IN)
			break
		except ValueError:
			print "You messed up. Try again idiot."

	#Enter Min Theta				
	print "Please enter minimum value for launch angle Theta."
	while(True):
		MinT = raw_input()
		try:
			MinT =  float(IN)
			break
		except ValueError:
			print "You messed up. Try again idiot."
	MinT = MinT % 360
	
	#Enter Max Theta				
	print "Please enter maximum value for launch angle Theta."
	while(True):
		MaxT = raw_input()
		try:
			MaxT =  float(IN)
			break
		except ValueError:
			print "You messed up. Try again idiot."
	MaxT = MaxT % 360
	RangeT = MaxT - MinT

	#Enter info for Phi
	print "Please enter angle resolution for launch angle Phi."
	while(True):
		Mp = raw_input()
		try:
			Mp =  int(IN)
			break
		except ValueError:
			print "You messed up. Try again idiot."

	#Enter Min Phi				
	print "Please enter minimum value for launch angle Phi."
	while(True):
		MinP = raw_input()
		try:
			MinP =  float(IN)
			break
		except ValueError:
			print "You messed up. Try again idiot."
	MinP = MinP % 360
	
	#Enter Max Phi				
	print "Please enter maximum value for launch angle Phi."
	while(True):
		MaxP = raw_input()
		try:
			MaxP =  float(IN)
			break
		except ValueError:
			print "You messed up. Try again idiot."
	MaxP = MaxP % 360
	RangeP = MaxP - MinP	
		
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
			CountP = Count+1
		CountTime = CountTime + 1

	#Write Possible Launch Points and Wait Times to a file
	##############################################################
	file = open("file.conf", "w")
	for item in Points:
		file.write("%s\n" % item)
	file.close()
	##############################################################	

print "**** Welcome to the Space Cannon Ballistics Simulation ****\n"

#User needs to input the TIME they are willing to wait till Rocket Launch. The total calculation will be split up into possible time periods in seconds.
print "Enter the maximum TIME you are willing to wait till Rocket Launch. \nThis will be entered as how many Days, Hours, and Minutes:"
print "How many DAYS?"	
while(True):
	days = raw_input()
	try:
		days = float(days)
		break
	except ValueError:
		print "You messed up. Try again idiot"

print "How many HOURS?"
while(True):
	hours = raw_input()
	try:
		hours = float(hours)
		break
	except ValueError:
		print "You messed up. Try again idiot"

print "How many MINUTES?"
while(True):
	minutes = raw_input()
	try:
		minutes = float(minutes)
		break
	except ValueError:
		print "You messed up. Try again idiot"

waittime = (int(days)*86400) + (int(hours)*3600) + (int(minutes)*60)

print "Now specify the resolution you would like to use to divide up the possible launch times."
while(True):
	TimeRes = raw_input()
	try:
		TimeRes = int(TimeRes)
		break
	except ValueError:
		print "Your messed up. Try again idiot" 

###############################################################################################################################################This is good
#User needs to input the time to WAIT for having the rocket hit the object (the total calculation will be in seconds.
print "\nEnter time willing to WAIT for object to travel.  \nThis will be entered as how many Days, Hours, and Minutes:"
print "How many DAYS?"
while(True):
	days = raw_input()
	try:
		days = float(days)
		break
	except ValueError:
		print "You messed up. Try again idiot"

print "How many HOURS?"
while(True):
	hours = raw_input()
	try:
		hours = float(hours)
		break
	except ValueError:
		print "You messed up. Try again idiot"

print "How many MINUTES?"
while(True):
	minutes = raw_input()
	try:
		minutes = float(minutes)
		break
	except ValueError:
		print "You messed up. Try again idiot"

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

Grid(TimeRes, waittime, traveltime) if(IN == "grid") else Point(TimeRes, waittime, traveltime)

############################################################################################################################################

print "\n**** Simulation ****"

# Run the C++ Space simulation executable file, named 'Space'	
os.system("./Space")

#This code calls the mpiexec command to spawn the MPIprocessfile.py file 2 times.//This will have to be an MPI Run command in the end, but MPI Exec is good for now//Might be alot more than 2 in the end
#command = "mpiexec -n 2 python ./MPIprocessfile.py"
#os.system(command)
#print ""








