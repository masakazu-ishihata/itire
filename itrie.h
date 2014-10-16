#ifndef _ITRIE_H_
#define _ITRIE_H_

/*------------------------------------*/
/* include */
/*------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <icsv.h>
#include <ilouds.h>

/* typedef */
typedef unsigned int ui;
typedef struct ITRIE itrie;
typedef struct ITRIENODE itrienode;

/*------------------------------------*/
/* itrie */
/*------------------------------------*/
struct ITRIE
{
  ui n;          /* Trie size */
  ui m;          /* alphabet size */
  itrienode *r;  /* root node */
  itrienode **l; /* node list */
};
/* new/free */
itrie *itrie_new(ui _m);
void itrie_free(void *_p);

/* export/import */
void itrie_export_matrix(FILE *_fp, itrie *_t);
void itrie_export_list(FILE *_fp, itrie *_t);
void itrie_export_louds(FILE *_fp, itrie *_t);
void itrie_export_dictionary(FILE *_fp, itrie *_t);
itrie *itrie_import_matrix(const char *_file);
itrie *itrie_import_list(const char *_file);
itrie *itrie_import_louds(const char *_file);

/* add, match */
itrienode *itrie_add(itrie *_t, ui _l, ui *_a);
itrienode *itrie_match(itrie *_t, ui _l, ui *_a);
void itrie_add_suffix(itrie *_t, ui _l, ui *_a);

/* accessors */
void itrie_make_table(itrie *_t);
void itrie_add_nodelist(itrie *_t, itrienode *_n);
itrienode *itrie_get_node(itrie *_t, ui _i);
ui **itrie_matrix(itrie *_t);

/*------------------------------------*/
/* itrie node */
/*------------------------------------*/
struct ITRIENODE
{
  itrie *trie;       /* trie */
  ui id;             /* node id */
  ui depth;          /* depth */
  ui value;          /* value */
  itrienode *parent; /* parent */
  itrienode **child; /* children */
};
/* new / free */
itrienode *itrienode_new(itrie *_t);
itrienode *itrienode_open(itrienode *_n, ui _i);
void itrienode_free(void *_p);

/* import/export */
void itrienode_export_matrix(FILE *_fp, itrienode *_n);
void itrienode_export_list(FILE *_fp, itrienode *_n);
void itrienode_export_louds(FILE *_fp, itrienode *_n);

/* add, match */
itrienode *itrienode_add(itrienode *_n, ui _l, ui *_a);
itrienode *itrienode_match(itrienode *_n, ui _l, ui *_a);
void itrienode_fill(itrienode *_n, ui **_A);

#endif
