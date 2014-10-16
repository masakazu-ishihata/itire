# itrie

Trie の実装。  
効率的かどうか怪しい。  
すべての文字は 0 以上の整数で表現される文字 ID に変化されていると仮定。  
文字 ID 列を add で Trie に追加し、match で最大 match する場所を返す。  

--------------------------------------------------------------------------------
## itrie

Trie 本体の実装。

### field

    ui n;
    ui m;
    itrienode *r;
    itrienode **l;

n は Trie 中の節点の総数。  
m は文字の種類数。  
r は根節点。  
l は(一時的な)節点のリスト。

### new, free

    itrie *itrie_new(ui _m);

アルファベットサイズ _m の Trie を生成する。

    void itrie_free(void *_p);

Trie _p を free する。

### export / import

    void itrie_export_matrix(FILE *_fp, itrie *_t);

Trie _t を行列形式で _fp に書き出す。  
出力の i 行 j 列目は節点 i の j 番目の子節点である。  
ただし 0 は対応する子節点が存在しないことを意味する。  

    void itrie_export_list(FILE *_fp, itrie *_t);

Trie _t をリスト _fp に書き出す。  
出力の i 行目の最初の数字は必ず i であり、以降 a:b 形式のリストである。  
ここで a:b は i の a 枝に対応する子は b であることを意味する。


    void itrie_export_louds(FILE *_fp, itrie *_t);

Trie _t を LOUDS 形式で _fp に書き出す。  
1行目は木構造を表現する LOUDS のビット列である。  
2行目の i 番目の数は LOUDS 中の節点 i の Trie での id である。  
3行めの i 万めの数は LOUDS 中の節点 i の Trie での value である。

    itrie *itrie_import_matrix(const char *_file);
    itrie *itrie_import_list(const char *_file);
    itrie *itrie_import_louds(const char *_file);

_file から各形式で出力された Trie を読み込む。

### accessors

    inodelist *itrie_add(itrie *_t, ui _l, ui *_a);

長さ _l の配列 _a を Trie _t に追加し、  
_a を表すパスの終点を返す。

    inodelist *itrie_match(itrie *_t, ui _l, ui *_a);

長さ _l の配列 _a を Trie _t から探し、  
最長マッチするパスの終点を返す。

    void itrie_add_suffix(itrie *_t, ui _l, ui *_a);

長さ _l の配列 _a のすべての suffix を Trie _t に追加する。  

    itrienode *itrie_get_node(itrie *_t, ui _i);

_i 番目の節点へのポインタを返す。  
この操作は節点リストを利用するため、  
最後に節点リスト作ったより後に add が行われている場合、  
内部的に節点リストの更新を行う。

### converter

    ui **itrie_matrix(itrie *_t);

tire _t を表現する _t->n * _t->m 行列を生成する。  
たとえば `A[i][j]` は節点 ID が i である節点の j 番目の子の ID を示す。

--------------------------------------------------------------------------------
## itrienode

Trie 木を構成する節点の実装。

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

長さ _l の配列 _a を節点 _n 以下に追加し、  
_a を表すパスの終点を返す。

    itrienode *itrienode_match(itrienode *_n, ui _l, ui *_a);

長さ _l の配列 _a を節点 _n 以下から探し、  
最長マッチするパスの終点を返す。


