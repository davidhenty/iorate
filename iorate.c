#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(void)
{
  // Data size in MiB

  const int nmeg = 128;
  
  int i;
  int comm, rank, size, namelen;
  char nodename[MPI_MAX_PROCESSOR_NAME];
  char *filename = "iorate.dat";
  
  double tstart, tstop;

  double *data;
  FILE *fp;

  const int dblesize = sizeof(double);
  const int kilo = 1024;
  const int mega = kilo*kilo;
  const int ndble = nmeg*mega;
  
  comm = MPI_COMM_WORLD;

  MPI_Init(NULL, NULL);

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  MPI_Get_processor_name(nodename, &namelen);

  if (rank == 0)
    {
      printf("Running iorate test on %d processes\n", size);
      printf("Testing serial IO rate from single process on node <%s>\n",
	     nodename);

      printf("Size of test array is %d MiB\n", (ndble * dblesize)/mega);

      data = (double *) malloc(ndble * dblesize);

      for (i=0; i < ndble; i++)
	{
	  data[i] = (double) i;
	}

      tstart = MPI_Wtime();

      fp = fopen(filename, "w");

      printf("Open time    = %f seconds\n", MPI_Wtime() - tstart);

      fwrite(data, dblesize, ndble, fp);

      printf("+ write time = %f seconds\n", MPI_Wtime() - tstart);

      fclose(fp);

      tstop = MPI_Wtime();

      printf("+ close time = %f seconds\n", tstop - tstart);

      remove(filename);

      printf("+ rm time    = %f seconds\n", tstop - tstart);

      printf("--------------------------------------------\n");
      printf("IO rate (excluding rm time) = %f MiB/s\n",
	     ((double) (ndble*dblesize)) / (((double) mega) * (tstop-tstart)));
      printf("--------------------------------------------\n");
      
    }
  
  MPI_Finalize();

  return 0;
}
  
