# itrie

Trie の実装

## itrienode

Trie 木を構成する節点。  

### new, free

    itrienode *itrienode_new(itrie *_t, int _depth, int _value);

Trie _t に属する節点を生成する。  
深さ _depth, ラベル _value。 

    itrienode *itrienode_open(itrienode *_n, int _i);

節点 _n の _i 番目の子を展開する。
自動的に対応する Trie のカウンタ等が増える。

    void itrienode_free(void *_p);

節点 _p 以下の部分木を free する。

    void itrienode_show(FILE *_fp, itrienode *_n);

節点 _p 以下の部分木を _fp に書き出す。

### add, match

    itrienode *itrienode_add(itrienode *_n, int _l, int *_a);

長さ _l の配列 _a を節点 _n 以下に追加する。
_a を表すパスの終点を返す。

    itrienode *itrienode_match(itrienode *_n, int _l, int *_a);

長さ _l の配列 _a を節点 _n 以下から探す。
最長マッチするパスの終点を返す。



## itrie

Trie 本体の実装

### new, fee

    itrie *itrie_new(int _m);

アルファベットサイズ _m の Trie を生成する。

    void itrie_free(void *_p);

Trie _p を free する。

### add, match

    itrienode *itrie_add(itrie *_t, int _l, int *_a);

長さ _l の配列 _a を Trie _t に追加する。  
_a を表すパスの終点を返す。

    itrienode *itrie_match(itrie *_t, int _l, int *_a);

長さ _l の配列 _a を Trie _t から探す。  
最長マッチするパスの終点を返す。

