/*
This code is from Chapter 12 of the book
"Algorithms in C, Third Edition,"
by Robert Sedgewick, Addison-Wesley, 1998.
*/

/* Definitions for the interface for items - file Item.h */

typedef int Item;
typedef int Key;

#define NULLitem -1 /* NULLitem is a constant */

#define key(A) (A)
#define less(A, B) (key(A) < key(B))
#define eq(A, B) (!less(A, B) && !less(B, A))

Item ITEMrand(void);
int ITEMscan(Item *);
void ITEMshow(Item);

/* Code for the implementation of the interface for items - file of type .c */

#include <stdio.h>
#include <stdlib.h>

int ITEMrand(void) { 
  return rand(); 
}

int ITEMscan(int*x) { 
  return scanf("%d", x); 
}

void ITEMshow(int x) { 
  printf("%5d ", x); 
}

/* Definitions for the interface for the symbol table - file ST.h */

void STinit();
int STcount();
void STinsert(Item);
Item STsearch(Key);
void STdelete(Item);
Item STselect(int);
void STsort(void (*visit)(Item));

/* Code for the implementation of the interface for symbol tables - file of type .c */

#include <stdlib.h>
// #include "Item.h"

typedef struct STnode { 
  Item item; 
  struct STnode* l;
  struct STnode* r;
  int N; 
} STnode;

static STnode* head;
static STnode* z;

STnode* NEW(Item item, STnode* l, STnode* r, int N) { 
  STnode* x = malloc(sizeof *x);
  x->item = item; 
  x->l = l; 
  x->r = r; 
  x->N = N;
  return x;
}

void STinit() { 
  head = (z = NEW(NULLitem, NULL, NULL, 0)); 
}

int STcount() { 
  return head->N; 
}

Item searchR(STnode* h, Key v) { 
  Key t = key(h->item);
  if (h == z) 
    return NULLitem;
  if eq(v, t) 
    return h->item;
  if less(v, t) 
    return searchR(h->l, v);
  else 
    return searchR(h->r, v);
}

Item STsearch(Key v) { 
  return searchR(head, v); 
}

STnode* insertR(STnode* h, Item item) { 
  Key v = key(item), t = key(h->item);
  if (h == z) 
    return NEW(item, z, z, 1);
  if less(v, t)
    h->l = insertR(h->l, item);
  else 
    h->r = insertR(h->r, item);
  return h;
}

void STinsert(Item item) { 
  head = insertR(head, item); 
}

void sortR(STnode* h, void (*visit)(Item)) {
  if (h == z) 
    return;
  sortR(h->l, visit);
  visit(h->item);
  sortR(h->r, visit);
}

void STsort(void (*visit)(Item)) { 
  sortR(head, visit); 
}

/* code for rotations */

STnode* rotR(STnode* h) { 
  STnode* x = h->l; h->l = x->r; x->r = h;
  return x; 
}

STnode* rotL(STnode* h) { 
  STnode* x = h->r; h->r = x->l; x->l = h;
  return x; 
}

/* code for finding the k-th smallest item in a BST */

Item selectR(STnode* h, int k) { 
  int t = h->l->N;
  if (h == z) 
    return NULLitem;
  if (t > k) 
    return selectR(h->l, k);
  if (t < k) 
    return selectR(h->r, k-t-1);
  return h->item;
}

Item STselect(int k) { 
  return selectR(head, k); 
}

/* code for partitioning a BST */

STnode* partR(STnode* h, int k) { 
  int t = h->l->N;

  if (t > k ) { 
    h->l = partR(h->l, k); 
    h = rotR(h); 
  }
  if (t < k ) { 
    h->r = partR(h->r, k-t-1); 
    h = rotL(h); 
  }

  return h;
}

/* code for deleting a key from a BST */

STnode* joinLR(STnode* a, STnode* b) {
  if (b == z) 
    return a;
  
  b = partR(b, 0); 
  b->l = a;
  return b;
}

STnode* deleteR(STnode* h, Key v) { 
  STnode* x; Key t = key(h->item);
  if (h == z) return z;
  if (less(v, t)) h->l = deleteR(h->l, v);
  if (less(t, v)) h->r = deleteR(h->r, v);
  if (eq(v, t))
    { x = h; h = joinLR(h->l, h->r); free(x); }
  return h;
}

void STdelete(Key v) { 
  head = deleteR(head, v); 
}

/* code for the main program */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include "Item.h"
// #include "ST.h"

int main(int argc, char *argv[]) { 
  int N, maxN = atoi(argv[1]), sw = atoi(argv[2]);
  Key v; Item item;
  srand(time(NULL));
  STinit();
  for (N = 0; N < maxN; N++)
    {
      if (sw) v = ITEMrand();
      else if (ITEMscan(&v) == EOF) break;
      if (STsearch(v) != NULLitem) continue;
      key(item) = v;
      STinsert(item);
    }
  STsort(ITEMshow); printf("\n");
  printf("%d keys ", N);
  printf("%d distinct keys\n", STcount());

  return 0;
}