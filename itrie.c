#include "itrie.h"

/*----------------------------------------------------------------------------*/
/* iTrie */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* new, free, show */
/*------------------------------------*/
itrie *itrie_new(int _m)
{
  itrie *_t = (itrie *)malloc(sizeof(itrie));
  _t->n = 0;
  _t->m = _m;
  _t->r = itrienode_new(_t, 0, -1);
  return _t;
}
void itrie_free(void *_p)
{
  itrie *_t = (itrie *)_p;
  if(_t != NULL){
    itrienode_free(_t->r);
    free(_t);
  }
}
void itrie_show(FILE *_fp, itrie *_t)
{
  fprintf(_fp, "<<<< Trie %p >>>>\n", _t);
  fprintf(_fp, "Alphabet size = %d\n", _t->m);
  fprintf(_fp, "Trie size     = %d\n", _t->m);
  fprintf(_fp, "Adress    Node ID   Depth     Value     Children\n");
  itrienode_show(_fp, _t->r);
}
/*------------------------------------*/
/* add, match */
/*------------------------------------*/
itrienode *itrie_add(itrie *_t, int _l, int *_a)
{
  return itrienode_add(_t->r, _l, _a);
}
itrienode *itrie_match(itrie *_t, int _l, int *_a)
{
  return itrienode_match(_t->r, _l, _a);
}

/*----------------------------------------------------------------------------*/
/* iTrie Node */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* new, free, show */
/*------------------------------------*/
/* new */
itrienode *itrienode_new(itrie *_t, int _depth, int _value)
{
  int i;
  itrienode *_n = (itrienode *)malloc(sizeof(itrienode));

  _n->trie  = _t;
  _n->id    = _t->n++;
  _n->depth = _depth;
  _n->value = _value;
  _n->child = (itrienode **)malloc(_t->m * sizeof(itrienode *));
  for(i=0; i<(_n->trie)->m; i++)
    _n->child[i] = NULL;

  return _n;
}
/* open */
itrienode *itrienode_open(itrienode *_n, int _i)
{
  if(_n->child[_i] == NULL)
    _n->child[_i] = itrienode_new(_n->trie, _n->depth+1, _i);

  return _n->child[_i];
}
/* free */
void itrienode_free(void *_p)
{
  int i;
  itrienode *_n = (itrienode *)_p;

  if(_n != NULL){
    for(i=0; i<(_n->trie)->m; i++)
      itrienode_free(_n->child[i]);
    free(_n->child);
    free(_n);
  }
}
/* show */
void itrienode_show(FILE *_fp, itrienode *_n)
{
  int i;
  fprintf(_fp, "%-10p%-10d%-10d%-10d", _n, _n->id, _n->depth, _n->value);

  for(i=0; i<(_n->trie)->m; i++)
    if(_n->child[i] != NULL) fprintf(_fp, "%d:%p ", i, _n->child[i]);
  fprintf(_fp, "\n");

  for(i=0; i<(_n->trie)->m; i++)
    if(_n->child[i] != NULL) itrienode_show(_fp, _n->child[i]);
}

/*------------------------------------*/
/* add, match */
/*------------------------------------*/
/* add : add _a to _n */
itrienode *itrienode_add(itrienode *_n, int _l, int *_a)
{
  /* nothing */
  if(_l == 0) return _n;

  /* expand */
  return itrienode_add(itrienode_open(_n, _a[0]), _l-1, &_a[1]);
}
/* match */
itrienode *itrienode_match(itrienode *_n, int _l, int *_a)
{
  int i = _a[0];
  return _l == 0 || _n->child[i] == NULL ? _n : itrienode_match(_n->child[i], _l-1, &(_a[1]));
}
