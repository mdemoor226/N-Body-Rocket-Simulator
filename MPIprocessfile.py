#MPIprocessfile.py
from mpi4py import MPI
#import numpy as np
import sys
import os
from subprocess import call
import subprocess
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()


def Shoot(Points):
	Alignment = 256#Alignment might be different on different machines
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
#Go over this process with Scott
if(rank == 0):
	#Read Launch Points from "file.conf" file
	file = open("file.conf", "r")
	Points = file.read().split("\n")
	file.close()

	#this removes the trailing /n from being an element
	Points = Points[:-1]
	
	print Points

else:
	Points = None

 
#Scatter array to each process
Points = comm.scatter(Points, root=0)

Points = Points[1:-1]
Points = Points.split(", ")

#print "Hello, my rank is: ", rank, " and my data is: ", Points

#Launch Shoot Traveltime Waittime Theta Phi
#If error in shoot throw exception (Most likely an error that will be common across all processes)#Check only on Master Node 
if(rank == 0):
	try:
		Shoot(Points)
	except Exception as E:
		print "Shoot Error: " + repr(E)
		#Terminate Program		
		comm.abort()
else:
	os.system("./Shoot " + str(Points[3]) + " " + str(Points[2]) + " " + str(Points[0]) + " " + str(Points[1]))		



#Parse Result.txt and Read in contents of Result.txt file
file = open("Result.txt", "r")
Line = file.readline()
if (Line[0:30] == "\"These input parameters suck!\""):
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

comm.Barrier()########################################Barrier####################################################################

#Gather Tracking Times from every simulation together into one array for the master node to compare times across simulations
TrackData = comm.gather(TrackTime, root=0)

OwnResult = 1 if(Status == "Success") else 0
StatusData = comm.gather(OwnResult, root=0)

#Determine if there were any successful simulations
comm.Reduce(OwnResult, RStatus, op=MPI.MAX, root=0)

RStatus = 0
BestRank = -1
if(rank == 0):
	if(RStatus == 1):
		Trackmin = sys.float_info.max
		Count = 0
		for S in StatusData:
			if(S == 1 and TrackData[Count] < Trackmin):
				Trackmin = TrackData[Count]
				BestRank = Count
			Count = Count + 1		

#Broadcast the Rank of the most successful Node
BestRank = comm.Bcast(BestRank, root=0)

if(BestRank != -1):
	if(rank == 0):
		if(BestRank == 0):
			#print out best possible input parameters
			print "..."
			
			#Terminate Program
		else:
			Data = recv(source=BestRank, tag=10)
			#Print out best possible input parameters
			print "..."

			#Terminate Program
	elif(rank == BestRank):
		#Send input Parameters back to master node
		comm.Send(Data, dest=0, tag=10)
		

Result = 0.0
comm.Reduce(Distance, Result, RStatus, op=MPI.MIN, root=0)
if(rank == 0):
	if(Result == sys.float_info.max):
		print "Your simulation setup really sucks." #Ultimate failure message		
		comm.abort()
		
		
DistData = comm.Gather(Distance, root=0)


#If any of the distances out of all the simulations are equal to the closest distance that one of the Rockets was able to get to its target planet (there has to be at least 1)
#and if the Time the Rocket took to get there is lower than all other simulation times where the the simulated Rockets came just as close
#then the node that obtained these results is the node with the best input parameters.
BestRank = -1
if(rank == 0):
	Count = 0
	Trackmin = sys.float_info.max
	for Dist in DistData:
		if(Dist == Result and TrackData[Count] < Trackmin):
			Trackmin = TrackData[Count]
			BestRank = Count
		Count = Count+1
		
	if(BestRank == 0):
		#print out best possible input parameters
		print "..."

		#Terminate Program
	
BestRank = comm.Broadcast(BestRank, root=0)	
	
if(rank == 0):
	Data = recv(source=BestRank, tag=11)
			
	#Master prints out the best possible input parameters
	print "...", Data	
	
	#Terminate Program

elif(BestRank == rank):
	send(Data, dest=0, tag=11)
		
		
		

#Implement a main function

#Call other functions inside main. Terminate program inside main		
		
		
