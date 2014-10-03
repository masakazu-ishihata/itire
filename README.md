# itrie

Trie の実装

--------------------------------------------------------------------------------
## itrie

Trie 本体の実装

### field

    ui n;
    ui m;
    itrienode *r;
    itrienode **l;

n は Trie 中の節点の数。  
m は文字の種類数。  
r は根節点。  
l は節点のリスト。

### new, free

    itrie *itrie_new(ui _m);

アルファベットサイズ _m の Trie を生成する。

    void itrie_free(void *_p);

Trie _p を free する。

### I/O

    void itrie_show(FILE *_fp, itrie *_t);

Trie _t を _fp に書き出す。

    void itrie_export(FILE *_fp, itrie *_t);

Trie _t を _fp に書き出す。

    itrie *itrie_import(const char *_file);

_file から Trie を読み込む。

### accessors

    inodelist *itrie_add(itrie *_t, ui _l, ui *_a);

長さ _l の配列 _a を Trie _t に追加する。  
_a を表すパスの終点を返す。

    inodelist *itrie_match(itrie *_t, ui _l, ui *_a);

長さ _l の配列 _a を Trie _t から探す。  
最長マッチするパスの終点を返す。

    void itrie_add_suffix(itrie *_t, ui _l, ui *_a);

長さ _l の配列 _a のすべての suffix を Trie _t に追加する。  

    itrienode *itrie_get_node(itrie *_t, ui _i);

_i 番目の節点へのポインタを返す。

### converter

    ui **itrie_matrix(itrie *_t);

tire _t を表現する _t->n * _t->m 行列を生成する。  
たとえば `A[i][j]` は節点 ID が i である節点の j 番目の子の ID を示す。

--------------------------------------------------------------------------------
## itrienode

Trie 木を構成する節点。


### field

    itrie *t;
    ui id;
    ui depth;
    ui value;
    itrinode *parent;
    itrienode **child;

t は自身が所属する Trie。  
id は節点 ID。  
depth は節点の深さ。  
value は自分の子としてID。  
parent は自身の親節点。  
child[i] は i 番目の子節点へのポインタ。

### new/free

    itrienode *itrienode_new(itrie *_t);

Trie _t に属する節点を生成する。  
_t の

    itrienode *itrienode_open(itrienode *_n, ui _i);

節点 _n の _i 番目の子を展開する。  
展開の有無に関わらず、_n の _i 番目の子を返す。

    void itrienode_free(void *_p);

節点 _p 以下の部分木を free する。

### I/O

    void itrienode_export(FILE *_fp, itrienode *_n);

節点 _n の子情報を _fp に書き出す。

### accessors

    itrienode *itrienode_add(itrienode *_n, ui _l, ui *_a);

長さ _l の配列 _a を節点 _n 以下に追加する。
_a を表すパスの終点を返す。

    itrienode *itrienode_match(itrienode *_n, ui _l, ui *_a);

長さ _l の配列 _a を節点 _n 以下から探す。
最長マッチするパスの終点を返す。


