#include "itrie.h"

/*----------------------------------------------------------------------------*/
/* iTrie */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* new / free */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* new */
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
/*------------------------------------*/
/* free */
/*------------------------------------*/
void itrie_free(void *_p)
{
  itrie *_t = (itrie *)_p;
  if(_t != NULL){
    itrienode_free(_t->r);
    free(_t->l);
    free(_t);
  }
}

/*----------------------------------------------------------------------------*/
/* export / import */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* export as a matrix */
/*------------------------------------*/
void itrie_export_matrix(FILE *_fp, itrie *_t)
{
  int i;
  for(i=0; i<_t->n; i++)
    itrienode_export_matrix(_fp, itrie_get_node(_t, i));
}
/*------------------------------------*/
/* export as a list */
/*------------------------------------*/
void itrie_export_list(FILE *_fp, itrie *_t)
{
  int i;
  for(i=0; i<_t->n; i++)
    itrienode_export_list(_fp, itrie_get_node(_t, i));
}
/*------------------------------------*/
/* export as a louds */
/*------------------------------------*/
void itrie_export_louds(FILE *_fp, itrie *_t)
{
  /* create bredth first list */
  if(_t->l != NULL) free(_t->l);
  _t->l = (itrienode **)calloc(_t->n, sizeof(itrienode *));

  int i, j, k=0;
  itrienode *c;
  _t->l[k++] = _t->r;
  for(i=0; i<_t->n; i++)
    for(j=0; j<_t->m; j++)
      if((c = (_t->l[i])->child[j]) != NULL)
        _t->l[k++] = c;

  /* export tree structure */
  for(i=_t->n-1; i>=0; i--)
    itrienode_export_louds(_fp, _t->l[i]);
  fprintf(_fp, "01\n");

  /* export ids */
  for(i=0; i<_t->n; i++)
    fprintf(_fp, " %d", (_t->l[i])->id);
  fprintf(_fp, "\n");

  /* export values */
  for(i=0; i<_t->n; i++)
    fprintf(_fp, " %d", (_t->l[i])->value);
  fprintf(_fp, "\n");

  /* free list */
  free(_t->l);
  _t->l = NULL;
}
/*------------------------------------*/
/* export list of words in _t */
/*------------------------------------*/
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

    free(a);
  }
}
/*------------------------------------*/
/* import matrix */
/*------------------------------------*/
itrie *itrie_import_matrix(const char *_file)
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
  for(i=0; i<N; i++)
    _t->l[i] = itrienode_new(_t);

  /* set parent-child relation */
  itrienode *n, *c;
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

  /* free */
  icsv_free(csv);

  return _t;
}
/*------------------------------------*/
/* improt list */
/*------------------------------------*/
itrie *itrie_import_list(const char *_file)
{
  icsv *csv = icsv_new_delimiter(_file, " ");
  int N = icsv_num_line(csv);
  int M = 0;
  int i, j, u, v;
  char word[256];

  /* get # charactors */
  for(i=0; i<N; i++){
    for(j=1; j<icsv_num_item(csv,i); j++){
      strcpy(word, icsv_get(csv, i, j));
      u = atoi( strtok(word, ":") );
      v = atoi( strtok(NULL, ":") );
      if(M < u) M = u;
    }
  }
  M++;

  /* new Trie */
  itrie *_t = (itrie *)malloc(sizeof(itrie));
  _t->n = 0;
  _t->m = M;
  _t->l = (itrienode **)calloc(N, sizeof(itrienode *));

  /* new Trie nodes */
  for(i=0; i<N; i++)
    _t->l[i] = itrienode_new(_t);

  /* set parent-child relation */
  itrienode *n, *c;
  for(i=0; i<N; i++){
    for(j=1; j<icsv_num_item(csv, i); j++){
      strcpy(word, icsv_get(csv, i, j));
      u = atoi( strtok(word, ":") ); /* u-th child */
      v = atoi( strtok(NULL, ":") ); /* node id */
      n = _t->l[i]; /* self */
      c = _t->l[v]; /* child */
      c->depth = n->depth + 1;
      c->value = u;
      c->parent = n;
      n->child[u] = c;
    }
  }
  _t->r = _t->l[0];

  /* free */
  icsv_free(csv);

  return _t;
}
/*------------------------------------*/
/* import louds */
/* _file should consis of 3 lines as follow */
/* line 0 : Tree structure     (2N items)   */
/* line 1 : id of each node    (N items)    */
/* line 3 : value of each node (N items)    */
/*------------------------------------*/
itrie *itrie_import_louds(const char *_file)
{
  icsv *csv = icsv_new_delimiter(_file, " ");
  int N = strlen( icsv_get(csv, 0, 0) ) / 2;
  int M = 0;
  int i, j, v;

  /* load M = # charactors */
  for(i=0; i<icsv_num_item(csv, 2); i++)
    if(M < (v = atoi( icsv_get(csv, 2, i) ) ))
      M = v;
  M++;

  /* load tree structure */
  ilouds *l = ilouds_import(icsv_get(csv, 0, 0) );

  /* new Trie */
  itrie *_t = (itrie *)malloc(sizeof(itrie));
  _t->n = 0;
  _t->m = M;
  _t->l = (itrienode **)calloc(N, sizeof(itrienode *));

  /* new Trie nodes */
  itrienode *n;
  for(i=0; i<N; i++){
    /* new */
    n = itrienode_new(_t);
    n->id    = atoi( icsv_get(csv, 1, i) );
    n->value = atoi( icsv_get(csv, 2, i) );
    n->depth = 0;

    /* set */
    _t->l[i] = n;
  }

  /* set parent-child relation */
  itrienode *c;
  for(i=0; i<N; i++){
    n = _t->l[i];

    /* get childlen */
    for(j=0; j<ilouds_get_num_children(l, i); j++){
      /* child */
      c = _t->l[ ilouds_get_child(l, i, j) ];

      /* set */
      n->child[ c->value ] = c;
      c->parent = n;
      c->depth = n->depth + 1;
    }
  }
  _t->r = _t->l[0];

  /* free */
  ilouds_free(l);
  icsv_free(csv);
  free(_t->l);
  _t->l = NULL;

  return _t;
}

/*----------------------------------------------------------------------------*/
/* accessors */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* add word _a to trie _t */
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
/*------------------------------------*/
/* find longest match */
/*------------------------------------*/
itrienode *itrie_match(itrie *_t, ui _l, ui *_a)
{
  return itrienode_match(_t->r, _l, _a);
}
/*------------------------------------*/
/* add all suffixs of _a to _t */
/*------------------------------------*/
void itrie_add_suffix(itrie *_t, ui _l, ui *_a)
{
  int i;
  for(i=0; i<_l; i++) itrie_add(_t, _l-i, &_a[i]);
}
/*------------------------------------*/
/* create a nodelist */
/*------------------------------------*/
void itrie_make_nodelist(itrie *_t)
{
  /* delete old list */
  if(_t->l != NULL) free(_t->l);

  /* make a list */
  _t->l = (itrienode **)malloc(_t->n * sizeof(itrienode *));
  itrie_add_nodelist(_t, _t->r);
}
/*------------------------------------*/
/* add node _n to nodelist */
/*------------------------------------*/
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
/*------------------------------------*/
/* get _i-th node in _t */
/*------------------------------------*/
itrienode *itrie_get_node(itrie *_t, ui _i)
{
  if(_t->l == NULL) itrie_make_nodelist(_t);
  return _t->l[_i];
}
/*------------------------------------*/
/* convert trie _t to matrix */
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
/*----------------------------------------------------------------------------*/
/* iTrie Node */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* new / free */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* new */
/*------------------------------------*/
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
/*------------------------------------*/
/* open */
/*------------------------------------*/
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
/*------------------------------------*/
/* free */
/*------------------------------------*/
void itrienode_free(void *_p)
{
  int i;
  itrienode *_n = (itrienode *)_p;

  if(_n != NULL){
    /* free children */
    for(i=0; i<(_n->trie)->m; i++) itrienode_free(_n->child[i]);
    free(_n->child);

    /* free self */
    free(_n);
  }
}

/*----------------------------------------------------------------------------*/
/* export */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* export as matrix */
/*------------------------------------*/
void itrienode_export_matrix(FILE *_fp, itrienode *_n)
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
/* export as list */
/*------------------------------------*/
void itrienode_export_list(FILE *_fp, itrienode *_n)
{
  int i;
  itrienode *c;

  fprintf(_fp, "%d", _n->id);
  for(i=0; i<(_n->trie)->m; i++)
    if((c = _n->child[i]) != NULL)
      fprintf(_fp, " %d:%d", i, c->id);
  fprintf(_fp, "\n");
}
/*------------------------------------*/
/* export louds */
/*------------------------------------*/
void itrienode_export_louds(FILE *_fp, itrienode *_n)
{
  int i;

  /* put 0 as end */
  fprintf(_fp, "0");

  /* put 1s same # as children */
  for(i=0; i<(_n->trie)->m; i++)
    if(_n->child[i] != NULL)
      fprintf(_fp, "1");
}
/*----------------------------------------------------------------------------*/
/* accessors */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* add _a to _n */
/*------------------------------------*/
itrienode *itrienode_add(itrienode *_n, ui _l, ui *_a)
{
  /* nothing */
  if(_l == 0) return _n;

  /* expand */
  return itrienode_add(itrienode_open(_n, _a[0]), _l-1, &_a[1]);
}
/*------------------------------------*/
/* match */
/*------------------------------------*/
itrienode *itrienode_match(itrienode *_n, ui _l, ui *_a)
{
  if(_l == 0) return _n;
  int i = _a[0];
  return _n->child[i] == NULL ? _n : itrienode_match(_n->child[i], _l-1, &(_a[1]));
}
/*------------------------------------*/
/* fill */
/*------------------------------------*/
void itrienode_fill(itrienode *_n, ui **_A)
{
  int i;
  itrienode *c;

  for(i=0; i<(_n->trie)->m; i++){
    if( (c = _n->child[i]) == NULL ) continue;
    _A[_n->id][i] = c->id;
    itrienode_fill(c, _A);
  }
}
