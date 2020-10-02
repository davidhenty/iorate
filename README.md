# iorate
Trivial C MPI program to test serial IO rates

Sample output on my laptop:

dsh@e7390dh:iorate$ mpicc -o iorate iorate.c
dsh@e7390dh:iorate$ mpirun -n 4 ./iorate
Running iorate test on 4 processes
Testing serial IO rate from single process on node <e7390dh>
Size of test array is 1024 MiB
Open time    = 0.000042 seconds
+ write time = 8.207337 seconds
+ close time = 8.207358 seconds
+ rm time    = 8.207358 seconds
--------------------------------------------
IO rate (excluding rm time) = 124.766090 MiB/s
--------------------------------------------
