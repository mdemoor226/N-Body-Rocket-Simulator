#MPIprocessfile.py
from mpi4py import MPI
import numpy as np
import sys


comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()


#Master Node scatters Launch Positions and Wait Times
#Each process receives scattered input parameters to determine its Wait time and Launch position(Launch Angles)
#Go over this process with Scott
if(rank == 0):
	Points = []
	#Parse through file.conf
	open("file.conf", "r")
	#Transform Numpy values into strings

else:
	Points = None


#...

#Scatter array to each process#Consider using numpy for this
Data = scatter('''...''')

#Launch Shoot Traveltime Waittime Theta Phi
#If error in shoot throw exception (Most likely an error that will be common across all processes)#Check only on Master Node 

#Parse Result.txt



comm.Barrier()########################################Barrier####################################################################

#Gather Tracking Times from every simulation together into one array for the master node to compare times across simulations
TrackData = comm.Gather(TrackTime, root=0)

OwnResult = 1 if(Status == "Success") else 0
StatusData = comm.Gather(OwnResult, root=0)

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
BestRank = comm.Broadcast(BestRank, root=0)

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
		send(Data, dest=0, tag=10)
		

Result = 0.0
comm.Reduce(Distance, Result, RStatus, op=MPI.MIN, root=0)
if(rank == 0):
	if(Result = sys.float_info.max):
		comm.abort()
		return "Your simulation setup really sucks." #Ultimate failure message
		
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

elif(BestRank == rank)
	send(Data, dest=0, tag=11)
		
		
		

#Implement a main function

#Call other functions inside main. Terminate program inside main		
		
		
