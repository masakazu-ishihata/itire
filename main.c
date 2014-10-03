#include "main.h"

#define N 100
#define V 10

int main(void)
{
  int i;

  ui a[N];
  for(i=0; i<N; i++) a[i] = i % V;


  itrie *T = itrie_new(V);
  itrie_add_suffix(T, N, a);

  FILE *fp = fopen("test.txt", "w");
  itrie_export(fp, T);
  fclose(fp);
  itrie_free(T);

  T = itrie_import("test.txt");
  itrie_export(stdout, T);
  itrie_free(T);

  return 0;
}
