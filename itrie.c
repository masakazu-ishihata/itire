#include "itrie.h"

/*----------------------------------------------------------------------------*/
/* iTrie */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* new/free */
/*------------------------------------*/
itrie *itrie_new(ui _m)
{
  itrie *_t = (itrie *)malloc(sizeof(itrie));
  _t->n = 0;
  _t->m = _m;
  _t->r = itrienode_new(_t);
  _t->l = NULL;
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
/*------------------------------------*/
/* I/O */
/*------------------------------------*/
void itrie_export(FILE *_fp, itrie *_t)
{
  int i;
  itrienode *n;

  for(i=0; i<_t->n; i++){
    n = itrie_get_node(_t, i);
    itrienode_export(_fp, n);
  }
}
void itrie_export_dictionary(FILE *_fp, itrie *_t)
{
  int i, j, d;
  itrienode *n;
  ui *a;

  for(i=1; i<_t->n; i++){
    /* get corresponding word (array of IDs) */
    n = itrie_get_node(_t, i);
    d = n->depth;
    a = (ui *)calloc(d, sizeof(ui));
    while(n->id > 0){
      a[n->depth-1] = n->value;
      n = n->parent;
    }

    /* export */
    for(j=0; j<d; j++) fprintf(_fp, " %d", a[j]);
    fprintf(_fp, "\n");
  }
}
itrie *itrie_import(const char *_file)
{
  icsv *csv = icsv_new_delimiter(_file, " ");
  int N = icsv_num_line(csv);
  int M = icsv_num_item(csv, 0);
  int i, j, k;

  /* new Trie */
  itrie *_t = (itrie *)malloc(sizeof(itrie));
  _t->n = 0;
  _t->m = M;
  _t->l = (itrienode **)malloc(N * sizeof(itrienode *));

  /* new Trienode */
  itrienode *n, *c;
  for(i=0; i<N; i++)
    _t->l[i] = itrienode_new(_t);

  for(i=0; i<N; i++){
    for(j=0; j<M; j++){
      k = atoi( icsv_get(csv, i, j) );
      if(k == 0) continue;
      n = _t->l[i];
      c = _t->l[k];
      c->depth = n->depth + 1;
      c->value = j;
      c->parent = n;
      n->child[j] = c;
    }
  }
  _t->r = _t->l[0];

  return _t;
}

/*------------------------------------*/
/* accessors */
/*------------------------------------*/
itrienode *itrie_add(itrie *_t, ui _l, ui *_a)
{
  /* free old list */
  if(_t->l != NULL){
    free(_t->l);
    _t->l = NULL;
  }

  /* add */
  return itrienode_add(_t->r, _l, _a);
}
itrienode *itrie_match(itrie *_t, ui _l, ui *_a)
{
  return itrienode_match(_t->r, _l, _a);
}
void itrie_add_suffix(itrie *_t, ui _l, ui *_a)
{
  int i;
  for(i=0; i<_l; i++) itrie_add(_t, _l-i, &_a[i]);
}
void itrie_make_nodelist(itrie *_t)
{
  /* delete old list */
  if(_t->l != NULL) free(_t->l);

  /* make a list */
  _t->l = (itrienode **)malloc(_t->n * sizeof(itrienode *));
  itrie_add_nodelist(_t, _t->r);
}
void itrie_add_nodelist(itrie *_t, itrienode *_n)
{
  int i;

  if(_n != NULL){
    /* add _n to list */
    _t->l[_n->id] = _n;

    /* add _n's children to list */
    for(i=0; i<_t->m; i++)
      itrie_add_nodelist(_t, _n->child[i]);
  }
}
itrienode *itrie_get_node(itrie *_t, ui _i)
{
  if(_t->l == NULL) itrie_make_nodelist(_t);
  return _t->l[_i];
}

/*------------------------------------*/
/* converter */
/*------------------------------------*/
ui **itrie_matrix(itrie *_t)
{
  int i;

  /* init */
  ui **A = (ui **)malloc(_t->n * sizeof(ui *));
  for(i=0; i<_t->n; i++)
    A[i] = (ui *)calloc(_t->m, sizeof(ui));

  /* fill */
  itrienode_fill(_t->r, A);

  return A;
}

/*----------------------------------------------------------------------------*/
/* iTrie Node */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* new/free */
/*------------------------------------*/
/* new */
itrienode *itrienode_new(itrie *_t)
{
  int i;
  itrienode *_n = (itrienode *)malloc(sizeof(itrienode));

  _n->trie   = _t;
  _n->id     = _t->n++;
  _n->depth  = 0;
  _n->value  = -1;
  _n->parent = NULL;
  _n->child  = (itrienode **)malloc(_t->m * sizeof(itrienode *));
  for(i=0; i<(_n->trie)->m; i++)
    _n->child[i] = NULL;

  return _n;
}
/* open */
itrienode *itrienode_open(itrienode *_n, ui _i)
{
  itrienode *c;

  /* open if not yet */
  if(_n->child[_i] == NULL){
    c = itrienode_new(_n->trie);
    c->depth = _n->depth + 1;
    c->value = _i;
    c->parent = _n;
    _n->child[_i] = c;
  }

  /* move to child */
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

/*------------------------------------*/
/* I/O */
/*------------------------------------*/
void itrienode_export(FILE *_fp, itrienode *_n)
{
  int i;
  itrienode *c;

  for(i=0; i<(_n->trie)->m; i++){
    c = _n->child[i];
    fprintf(_fp, " %d", c == NULL ? 0 : c->id);
  }
  fprintf(_fp, "\n");
}

/*------------------------------------*/
/* accessors */
/*------------------------------------*/
/* add : add _a to _n */
itrienode *itrienode_add(itrienode *_n, ui _l, ui *_a)
{
  /* nothing */
  if(_l == 0) return _n;

  /* expand */
  return itrienode_add(itrienode_open(_n, _a[0]), _l-1, &_a[1]);
}
/* match */
itrienode *itrienode_match(itrienode *_n, ui _l, ui *_a)
{

  if(_l == 0) return _n;
  int i = _a[0];
  return _n->child[i] == NULL ? _n : itrienode_match(_n->child[i], _l-1, &(_a[1]));
}
/* fill */
void itrienode_fill(itrienode *_n, ui **_A)
{
  int i;
  itrienode *c;

  for(i=0; i<(_n->trie)->m; i++){
    c = _n->child[i];
    if(c != NULL){
      _A[_n->id][i] = c->id;
      itrienode_fill(c, _A);
    }
  }
}
