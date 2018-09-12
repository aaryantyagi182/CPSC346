/*
Threads, struct
Purpose: To demonstrate how to pass multiple arguments to a thread using struct
         and how to decompose a problem into threads
Notes:
  This program uses one thread to compute prime numbers and another to waste 
  time opening a closing a file.  The number of primes, the number or openings
  and closings, and the file names come from the command line. The file that is 
  being opened and closed must previously exist.  
  The program demonstrates separating functions into threads, so that, 
  for example, a compute intensive functions does not wait for an i/o intensive
  function. 
  The four expected parameters, in order, are number of primes to be computed, 
  the file name to hold the primes, the number of opens and closes, the name 
  of the pre-existing file to be opened and closed. 
Usage: ex13 10 primeFile 100 inFile 
Output: file containing primes generated by the program
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* f1(void*);
void* f2(void*);
int is_prime(int);

struct params
{
 int num;
 char* file_name;
};

typedef struct params param;
 
int main(int argc, char* argv[])
{
  pthread_t t1,t2;
  param p1,p2;
  int n, status;

  p1.num = atoi(argv[1]);
  n = strlen(argv[2] + 1);
  p1.file_name = calloc(n, sizeof(char));
  strcpy(p1.file_name,argv[2]);
  status = pthread_create(&t1, NULL, f1, (void*)&p1 );
  if (status != 0)
  {
    printf("Error in thread 1:  %d\n",status);
    exit(-1);
  }

  p2.num = atoi(argv[3]);
  n = strlen(argv[4] + 1);
  p2.file_name = calloc(n, sizeof(char));
  strcpy(p2.file_name,argv[4]);
  status = pthread_create(&t2, NULL, f2, (void*)&p2 );
  if (status != 0)
  {
    printf("Error in thread 2:  %d\n",status);
    exit(-1);
  }
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  return 0; 
}

void* f1(void* param_in) 
{
  printf("entering function 1\n");
  param* p = (param*)param_in;
  FILE  *fp;
  fp = fopen(p->file_name, "w"); 
  int i; 
  fprintf(fp,"%d\n",2);
  for (i=3; i <= p->num; i++)
   if (is_prime(i) != 0)  
       fprintf(fp,"%d\n",i);
  fclose(fp); 
  printf("leaving function 1\n");
  pthread_exit(NULL);
}

int is_prime(int num)
{
 int i = 2;
 while (i < num)
 {
  if (num %2 == 0)
     return 0;
  i++;
 }
 return 1;
} 

void* f2(void* param_in) 
{
  printf("entering function 2\n");
  param* p = (param*)param_in;
  FILE  *fp;
  int i = 0;
  while (i < p->num)
   {
    fp = fopen(p->file_name, "r");
    char ch = getc(fp);
    fclose(fp);
    ++i;
   }
  printf("leaving function 2\n");
  pthread_exit(NULL);
} 