#MPIprocessfile.py
from mpi4py import MPI 
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

print "Hello world from process %d out of %d! " % (rank, size)

#Master Node scatters Launch Positions and Wait Times

#Each process receives scattered input parameters to determine its Wait time and Launch position(Launch Angles)
#Travel time is universal across all processes





#I'm not sure if Simulation.conf (outputted by first exectable 'Space') needs to be broadcasted to each node or if each node will already have access to it






#Each process needs to run ./Shoot TravelTime WaitTime LaunchAngleTheta LaunchAnglePhi






#Call ALLREDUCE on values in Result.txt (outputted by second executable 'Shoot') to determine best input parameters (Launch Angles and Wait Time)






#Process with best input parameters sends them back to Master Node






#Master Node prints out best input parameters to user
