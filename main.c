#include "main.h"

#define N 256

int main(void)
{
  int i;

  /* string */
  char str[N] = "suffix";
  char file[N] = "trie.txt";

  /* string -> ID list */
  ui n = strlen(str);
  ui *a = (ui *)calloc(n, sizeof(ui));

  ui min = str[0];
  for(i=1; i<n; i++) if(min > str[i]) min = str[i];

  ui max = 0;
  for(i=0; i<n; i++){
    a[i] = str[i] - min;
    if(max < a[i]) max = a[i];
  }

  /* show */
  printf("\"%s\" =", str);
  for(i=0; i<n; i++) printf("%3d", a[i]); printf("\n");

  /* new */
  itrie *T = itrie_new(max + 1);
  itrie_add_suffix(T, n, a);
  free(a);

  /* export */
  FILE *fp = fopen(file, "w");
  printf("<<<< Trie (constructed) >>>>\n");
  itrie_export_list(stdout, T);
  itrie_export_louds(fp, T);
  fclose(fp);
  itrie_free(T);

  /* import */
  T = itrie_import_louds(file);
  printf("<<<< Trie (imported) >>>>\n");
  itrie_export_list(stdout, T);
  printf("<<<< dictionary >>>>\n");
  itrie_export_dictionary(stdout, T);
  itrie_free(T);

  return 0;
}
