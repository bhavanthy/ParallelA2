##
## Build our test suite
##


sample : 
	mpicc a2.c -o a2 


## get rid of all produced files
clean :
	@ echo "cleaning up!"
	- rm -f *.o 
