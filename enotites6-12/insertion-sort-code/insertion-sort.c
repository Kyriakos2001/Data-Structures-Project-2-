/* 
  This code originated from the book "Algorithms in C" by Robert Sedgewick.
  Modified by: 
  - Sergios - Anestis Kefalidis
  - Konstantinos Nikoletos
  - Kostas Plas
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int Item;

void sort(Item a[], int l, int r)
{ 
  for (int i = l+1; i <= r; i++) {
    if (a[i] < a[l]) {
      Item t = a[i];
      a[i] = a[l];
      a[l] = t;
    }
  }

  for (int i = l+2; i <= r; i++) { 
    int j = i; 
    Item v = a[i];
    while (v < a[j-1]) { 
      a[j] = a[j-1]; 
      j--; 
    }
    a[j] = v;
  }
}

int main(int argc, char *argv[])
{ 
  // initialize the random number generator
  srand(time(NULL));

  int N = atoi(argv[1]);
  Item *a = malloc(N*sizeof(Item));
  for (int i = 0; i < N; i++)
    a[i] = 1000*(1.0*rand()/RAND_MAX); // [0, 1000]

  for (int i = 0; i < N; i++) 
    printf("%3d ", a[i]);
  printf("\n");

  sort(a, 0, N-1);

  for (int i = 0; i < N; i++) 
    printf("%3d ", a[i]);
  printf("\n");

  free(a);

  return 0;
}
