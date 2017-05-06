/* Application Benchmark Test Package Timer
 * Tyler Simon and Tom Oppe
 * 8/10/2006
 * Get MPI startup times
 */


#include <sys/time.h>
#include <stdio.h>
#include <mpi.h>
double tbeg;

int test(argc, argv)
int argc;
char **argv;
{
 int i;
 double a[200000];
 MPI_Init(&argc,&argv);
 for (i=0; i<200000; i++) {
   a[i] = (double) i;
 }
 printf(" a(100) = %10.5f\n", a[100]);
 MPI_Finalize();
}
      
int MPI_Init (int *argc, char ***argv)
{
 PMPI_Init(argc, argv);
 tbeg = MPI_Wtime();
 return 0;
}

int MPI_Finalize(void)
{
 int comm, i, i_tbeg_max, i_tbeg_min, i_tend_max, i_tend_min;
 int i_telp_max, i_telp_min, len_name, maxlen, mchr, mdbl;
 int myid, nprocs;
 double tb,       tbeg_max, tbeg_min, tbeg_avg, tbeg_sum;
 double te, tend, tend_max, tend_min, tend_avg, tend_sum;
 double     telp, telp_max, telp_min, telp_avg, telp_sum;
 double ftimer1, ftimer2;
 double getCurrentTime(void);
 char pname[MPI_MAX_PROCESSOR_NAME];
 char *pname_all;
 double *tbeg_all, *tend_all;
 tend = MPI_Wtime();
 comm = MPI_COMM_WORLD;
 MPI_Comm_rank (comm, &myid);
 if (myid == 0){
   ftimer1 = getCurrentTime();
 }
 maxlen = MPI_MAX_PROCESSOR_NAME;
 MPI_Get_processor_name(pname, &len_name);
 mdbl = MPI_DOUBLE;
 mchr = MPI_CHAR;
 if (myid == 0){
   MPI_Comm_size (comm, &nprocs);
   tbeg_all = (double*) malloc (nprocs*sizeof(double));
   tend_all = (double*) malloc (nprocs*sizeof(double));
   pname_all = (char*) malloc (nprocs*maxlen*sizeof(char));
 }
 MPI_Gather ((void *) &tbeg, 1, mdbl, (void *) tbeg_all, 1, mdbl, 0, comm);
 MPI_Gather ((void *) &tend, 1, mdbl, (void *) tend_all, 1, mdbl, 0, comm);
 MPI_Gather ((void *) pname, maxlen, mchr, (void *) pname_all, maxlen, mchr, 0, comm);
 if (myid == 0){
   tbeg_sum = 0.0;
   tend_sum = 0.0;
   telp_sum = 0.0;
   tbeg_max = tbeg_all[0];
   tbeg_min = tbeg_all[0];
   tend_max = tend_all[0];
   tend_min = tend_all[0];
   telp_max = tend_all[0] - tbeg_all[0];
   telp_min = tend_all[0] - tbeg_all[0];
   i_tbeg_max = 0;
   i_tbeg_min = 0;
   i_tend_max = 0;
   i_tend_min = 0;
   i_telp_max = 0;
   i_telp_min = 0;
   for (i=0; i<nprocs; i++) {
     tb = tbeg_all[i];
     te = tend_all[i];
     telp = te - tb;
     tbeg_sum = tbeg_sum + tb;
     tend_sum = tend_sum + te;
     telp_sum = telp_sum + telp;
     if (tb > tbeg_max) {
       tbeg_max = tb;
       i_tbeg_max = i;
     }
     if (tb < tbeg_min) {
       tbeg_min = tb;
       i_tbeg_min = i;
     }
     if (te > tend_max) {
       tend_max = te;
       i_tend_max = i;
     }
     if (te < tend_min) {
       tend_min = te;
       i_tend_min = i;
     }
     if (telp > telp_max) {
       telp_max = telp;
       i_telp_max = i;
     }
     if (telp < telp_min) {
       telp_min = telp;
       i_telp_min = i;
     }
   }
   tbeg_avg = tbeg_sum/((double) nprocs);
   tend_avg = tend_sum/((double) nprocs);
   telp_avg = telp_sum/((double) nprocs);
   printf(" ++++ TIMER ++++:  *** Begin Timing Report ***\n\
 ++++ TIMER ++++:\n\
 ++++ TIMER ++++:\n\
 ++++ TIMER ++++:  Max MPI_Init timer val = %22.6f at rank %7d\n\
 ++++ TIMER ++++:  Min MPI_Init timer val = %22.6f at rank %7d\n\
 ++++ TIMER ++++:  Avg MPI_Init timer val = %22.6f\n\
 ++++ TIMER ++++:\n\
 ++++ TIMER ++++:  Max MPI_Fin  timer val = %22.6f at rank %7d\n\
 ++++ TIMER ++++:  Min MPI_Fin  timer val = %22.6f at rank %7d\n\
 ++++ TIMER ++++:  Avg MPI_Fin  timer val = %22.6f\n\
 ++++ TIMER ++++:\n\
 ++++ TIMER ++++:  Max Fin-Init timer val = %22.6f at rank %7d\n\
 ++++ TIMER ++++:  Min Fin-Init timer val = %22.6f at rank %7d\n\
 ++++ TIMER ++++:  Avg Fin-Init timer val = %22.6f\n\
 ++++ TIMER ++++:\n\
 ++++ TIMER ++++:  Max Fin - Min Init val = %22.6f\n\
 ++++ TIMER ++++:  Min Fin - Max Init val = %22.6f\n\
 ++++ TIMER ++++:\n\
 ++++ TIMER ++++:     Rank           Elapsed Time               MPI_Init\
           MPI_Finalize       Processor Name\n",
 tbeg_max, i_tbeg_max, tbeg_min, i_tbeg_min, tbeg_avg, 
 tend_max, i_tend_max, tend_min, i_tend_min, tend_avg, 
 telp_max, i_telp_max, telp_min, i_telp_min, telp_avg,
 tend_max - tbeg_min, tend_min - tbeg_max);
   for (i=0; i<nprocs; i++) {
     tb = tbeg_all[i];
     te = tend_all[i];
     telp = te - tb;
     printf(" ++++ TIMER ++++: %8d %22.6f %22.6f %22.6f       %s\n",
          i, telp, tb, te, &pname_all[maxlen*i]);
   }
   printf(" ++++ TIMER ++++:\n\
 ++++ TIMER ++++:  TIMER Overhead (inside MPI) from Rank 0     = %18.6f\n",
 MPI_Wtime() - tend);
   free (tbeg_all);
   free (tend_all);
   free (pname_all);
 }
 PMPI_Finalize();
 if (myid == 0){
   ftimer2 = getCurrentTime();
   printf(" ++++ TIMER ++++:  TIMER and MPI_Finalize Overhead from Rank 0 = %18.6f\n\
 ++++ TIMER ++++:\n\
 ++++ TIMER ++++:\n\
 ++++ TIMER ++++:  *** End TIMER Timing Report ***\n", ftimer2 - ftimer1);
 }
 return 0;
}

double getCurrentTime(void){
  struct timeval tval;
  gettimeofday(&tval, NULL);
  return (tval.tv_sec + tval.tv_usec/1000000.0); }
