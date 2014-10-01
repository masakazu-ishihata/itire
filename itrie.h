#ifndef _ITRIE_H_
#define _ITRIE_H_

/*------------------------------------*/
/* include */
/*------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

/* typedef */
typedef unsigned int ui;
typedef struct ITRIE itrie;
typedef struct ITRIENODE itrienode;

/*------------------------------------*/
/* itrie */
/*------------------------------------*/
struct ITRIE
{
  int n;        /* Trie size */
  int m;        /* alphabet size */
  itrienode *r; /* root node */
};
/* new, free, show */
itrie *itrie_new(int _m);
void itrie_free(void *_p);
void itrie_show(FILE *_fp, itrie *_t);
/* add, match */
itrienode *itrie_add(itrie *_t, int _l, int *_a);
itrienode *itrie_match(itrie *_t, int _l, int *_a);


/*------------------------------------*/
/* itrie node */
/*------------------------------------*/
struct ITRIENODE
{
  itrie *trie;       /* trie */
  int id;            /* node id */
  int depth;         /* depth */
  int value;         /* value */
  itrienode **child; /* child */
};
/* new, free, show */
itrienode *itrienode_new(itrie *_t, int _depth, int _value);
itrienode *itrienode_open(itrienode *_n, int _i);
void itrienode_free(void *_p);
void itrienode_show(FILE *_fp, itrienode *_n);

/* add, match */
itrienode *itrienode_add(itrienode *_n, int _l, int *_a);
itrienode *itrienode_match(itrienode *_n, int _l, int *_a);

#endif
