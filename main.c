#include "main.h"

#define N 3

int main(void)
{
  int a[N] = {0, 1, 2};
  int b[N] = {0, 1, 1};
  int c[N] = {0, 1, 3};

  itrie *t = itrie_new(10);
  itrienode *n;
  itrie_show(stdout, t);

  itrie_add(t, N, a);
  itrie_add(t, N, b);
  n = itrie_match(t, N, c);
  printf("match %d charactors\n", n->depth);

  itrie_show(stdout, t);
  itrie_free(t);

  return 0;
}
