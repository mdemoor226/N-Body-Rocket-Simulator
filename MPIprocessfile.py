#MPIprocessfile.py
from mpi4py import MPI
import numpy as np
import sys
import os
from subprocess import call
import subprocess
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()


#This function calls the C++ Shoot program.  Each node (including the root node) will run this with diff data sets.
#Error handling is included
def Shoot(Points):
	Alignment = 256
	value = os.system("./Shoot " + str(Points[3]) + " " + str(Points[2]) + " " + str(Points[0]) + " " + str(Points[1]))	
	Error = value/Alignment
	if(Error == 1):
		raise Exception("Invalid arg num for shoot")
	elif(Error == 2):
		raise Exception("Inavlid args for shoot")
	elif(Error == 3):
		raise Exception("File errors in shoot")
	elif(Error == 4):
		raise Exception("Parsing errors in shoot")
	else:
		return


#Master Node scatters Launch Positions and Wait Times
#Each process receives scattered input parameters to determine its Wait time and Launch position(Launch Angles)
if(rank == 0):
	#Read Launch Points from "file.conf" file
	file = open("file.conf", "r")
	Points = file.read().split("\n")
	file.close()

	#this removes the trailing /n from being an element
	Points = Points[:-1]

else:
	Points = None

 
#Scatter array to each process
Points = comm.scatter(Points, root=0)

#print "Hello, my rank is: ", rank, " and my data is: ", Points
Points = Points[1:-1]
Points = Points.split(", ")

#Launch Shoot Traveltime Waittime Theta Phi
#If error in shoot throw exception (Most likely an error that will be common across all processes)#Check only on Master Node 
if(rank == 0):
	try:
		Shoot(Points)
	except Exception as E:
		print "Shoot Error: " + repr(E)
		#Terminate Program		
		comm.Abort()
else:
	os.system("./Shoot " + str(Points[3]) + " " + str(Points[2]) + " " + str(Points[0]) + " " + str(Points[1]))
		
comm.Barrier()########################################Barrier####################################################################

#Parse Result.txt and Read in contents of Result.txt file
file = open("Result.txt", "r")
Line = file.readline()

if (Line[0:28] == "These input parameters suck!"):
	Distance = sys.float_info.max
	TrackTime = sys.float_info.max
	Status = "Failure"
	file.close()

else:
	Results = Line.split(" ")
	Distance = Results[0]
	TrackTime = Results[1]
	Status = Results[2]
	file.close()
 
OwnResult = []

#Gather Tracking Times from every simulation together into one array for the master node to compare times across simulations
TrackData = comm.gather(TrackTime, root=0)

OwnResult = 1 if(Status == "Success") else 0
StatusData = comm.gather(OwnResult, root=0)

#Determine if there were any successful simulations
comm.reduce(OwnResult, op=MPI.MAX, root=0)

BestRank = -1
if(rank == 0):
	if(OwnResult == 1):
		Trackmin = sys.float_info.max
		Count = 0
		for S in StatusData:
			if(S == 1 and TrackData[Count] < Trackmin):
				Trackmin = TrackData[Count]
				BestRank = Count
			Count = Count + 1		

#Broadcast the Rank of the most successful Node
BestRank = comm.bcast(BestRank, root=0)


if(BestRank != -1):
	if(rank == 0):
		if(BestRank == 0):
			#print out best possible input parameters
			print ""
			print "Closest Distance Achieved from Rocket to Target: ",Distance
			print "#####################################################################"
			print "FINAL RESULTS FOR ROCKET SIMULATION:		            ########"
			print "#####################################################################"
			print "The Best Input Parameters Include:"
			print "Launch Angle THETA from the planet:        ", Points[0], "degrees"
			print "Launch Angle PHI from the planet:          ", Points[1], "degrees"
			print "Ideal time to wait before rocket launch:   ", Points[2], "seconds"
			print "Travel time from launch to rocket impact:  ", Points[3], "seconds"
			print "#####################################################################"
			print "#####################################################################"
			print ""			
			#Terminate Program
			comm.Abort()
		else:
			Data = recv(source=BestRank, tag=10)
			print ""
			print "Closest Distance Achieved from Rocket to Target: ",Data[4]
			print "#####################################################################"
			print "FINAL RESULTS FOR ROCKET SIMULATION:		            ########"
			print "#####################################################################"
			print "The Best Input Parameters Include:"
			print "Launch Angle THETA from the planet:        ", Data[0], "degrees"
			print "Launch Angle PHI from the planet:          ", Data[1], "degrees"
			print "Ideal time to wait before rocket launch:   ", Data[2], "seconds"
			print "Travel time from launch to rocket impact:  ", Data[3], "seconds"
			print "#####################################################################"
			print "#####################################################################"
			print ""
			#Terminate Program
			comm.Abort()
	elif(rank == BestRank):
		#Send input Parameters back to master node
		Points.append(Distance)
		comm.Send(Points, dest=0, tag=10)
		
Result = np.zeros(1)
D = np.zeros(1)
D[0] = Distance

#Reduce to get the main result Result of Minimum
comm.Reduce(D, Result, op=MPI.MIN, root=0)
Result = float(Result)

if(rank == 0):
	if(Result == sys.float_info.max):
		print "Your simulation setup really sucks." #Ultimate failure message		
		comm.Abort()

#Gather the results and pass it to the root node		
DistData = comm.gather(Distance, root=0)


#If any of the distances out of all the simulations are equal to the closest distance that one of the Rockets was able to get to its target planet (there has to be at least 1)
#and if the Time the Rocket took to get there is lower than all other simulation times where the the simulated Rockets came just as close
#then the node that obtained these results is the node with the best input parameters.
BestRank = -1
if(rank == 0):	
	Count = 0
	Trackmin = sys.float_info.max
	for Dist in DistData:
		if((float(Dist) == float(Result)) and (float(TrackData[Count]) < float(Trackmin))):
			Trackmin = TrackData[Count]
			BestRank = Count
		Count = Count+1

#Broadcast the Rank of the Best Node from the root
BestRank = comm.bcast(BestRank, root=0)	

if(rank == 0):
	if(BestRank == 0):
		print ""
		print "Closest Distance Achieved from Rocket to Target: ",Distance		
		print "#####################################################################"
		print "FINAL RESULTS FOR ROCKET SIMULATION:		            ########"
		print "#####################################################################"
		print "The Best Input Parameters Include:"
		print "Launch Angle THETA from the planet:        ", Points[0], "degrees"
		print "Launch Angle PHI from the planet:          ", Points[1], "degrees"
		print "Ideal time to wait before rocket launch:   ", Points[2], "seconds"
		print "Travel time from launch to rocket impact:  ", Points[3], "seconds"
		print "#####################################################################"
		print "#####################################################################"
		print ""

	else:
		Data = comm.recv(source=BestRank, tag=11)
		
		#Master prints out the best possible input parameters
		print ""
		print "Closest Distance Achieved from Rocket to Target: ",Data[4]
		print "#####################################################################"
		print "FINAL RESULTS FOR ROCKET SIMULATION:		            ########"
		print "#####################################################################"
		print "The Best Input Parameters Include:"
		print "Launch Angle THETA from the planet:        ", Data[0], "degrees"
		print "Launch Angle PHI from the planet:          ", Data[1], "degrees"
		print "Ideal time to wait before rocket launch:   ", Data[2], "seconds"
		print "Travel time from launch to rocket impact:  ", Data[3], "seconds"
		print "#####################################################################"
		print "#####################################################################"	
		print ""

elif(BestRank == rank):
	Points.append(Distance)
	comm.send(Points, dest=0, tag=11)
		
				
