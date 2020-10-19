#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(void)
{
  // Data size in GiB

  const int ngig = 4;
  
  int i;
  int rank, size, namelen;
  MPI_Comm comm;
  char nodename[MPI_MAX_PROCESSOR_NAME];
  char *filename = "iorate.dat";
  
  double tstart, tstop;

  double *data;
  FILE *fp;

  const size_t dblesize = sizeof(double);
  const size_t kilo = 1024;
  const size_t mega = kilo*kilo;
  const size_t giga = kilo*mega;

  const size_t ndble = ngig*giga;
  
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

      printf("Size of test array is %ld MiB\n", dblesize*(ndble/mega));

      data = (double *) malloc(ndble * dblesize);

      for (i=0; i < ndble; i++)
	{
	  data[i] = (double) i;
	}

      printf("\nTesting write\n");
      printf("-------------\n");

      tstart = MPI_Wtime();

      fp = fopen(filename, "w");

      printf("Open time    = %f seconds\n", MPI_Wtime() - tstart);

      fwrite(data, dblesize, ndble, fp);

      printf("+ write time = %f seconds\n", MPI_Wtime() - tstart);

      fclose(fp);

      tstop = MPI_Wtime();

      printf("+ close time = %f seconds\n", tstop - tstart);

      printf("--------------------------------------------\n");
      printf("IO rate (excluding rm time) = %f GiB/s\n",
	     ((double) ndble * (double) dblesize) / (((double) giga) * (tstop-tstart)));
      printf("--------------------------------------------\n");
      
      printf("\nTesting fread\n");
      printf("------------\n");

      tstart = MPI_Wtime();

      fp = fopen(filename, "r");

      printf("Open time    = %f seconds\n", MPI_Wtime() - tstart);

      fread(data, dblesize, ndble, fp);

      printf("+ fread time = %f seconds\n", MPI_Wtime() - tstart);

      fclose(fp);

      tstop = MPI_Wtime();

      printf("+ close time = %f seconds\n", tstop - tstart);

      remove(filename);

      printf("+ rm time    = %f seconds\n", MPI_Wtime() - tstart);

      printf("--------------------------------------------\n");
      printf("IO rate (excluding rm time) = %f GiB/s\n",
	     ((double) ndble * (double) dblesize) / (((double) giga) * (tstop-tstart)));
      printf("--------------------------------------------\n");
    }
  
  MPI_Finalize();

  return 0;
}
  
