#include <iostream>
using namespace std;

constexpr int maxn = 1e5 + 10;
constexpr int mod = 51061;

struct Node {
    int ls, rs, fa, size;
    bool rev;
    int key, sum, add, mul;

    int &operator[](int x) {
        return (x == 0 ? ls : rs);
    }
};

int n, q;
Node tree[maxn];
int st[maxn];

void pushup(int x) {
    tree[x].sum = (tree[tree[x].ls].sum + tree[x].key + tree[tree[x].rs].sum) % mod;
    tree[x].size = tree[tree[x].ls].size + 1 + tree[tree[x].rs].size;
}

void updateReverse(int x) {
    tree[x].rev ^= 1;
    swap(tree[x].ls, tree[x].rs);
}

void updateAdd(int x, int v) {
    tree[x].key = (tree[x].key + v) % mod;
    tree[x].sum = (tree[x].sum + 1ll * tree[x].size * v) % mod;
    tree[x].add = (tree[x].add + v) % mod;
}

void updateMultiply(int x, int v) {
    tree[x].key = 1ll * tree[x].key * v % mod;
    tree[x].sum = 1ll * tree[x].sum * v % mod;
    tree[x].add = 1ll * tree[x].add * v % mod;
    tree[x].mul = 1ll * tree[x].mul * v % mod;
}

void pushdown(int x) {
    if (tree[x].rev) {
        if (tree[x].ls)
            updateReverse(tree[x].ls);

        if (tree[x].rs)
            updateReverse(tree[x].rs);

        tree[x].rev = false;
    }

    if (tree[x].mul != 1) {
        if (tree[x].ls)
            updateMultiply(tree[x].ls, tree[x].mul);

        if (tree[x].rs)
            updateMultiply(tree[x].rs, tree[x].mul);

        tree[x].mul = 1;
    }

    if (tree[x].add) {
        if (tree[x].ls)
            updateAdd(tree[x].ls, tree[x].add);

        if (tree[x].rs)
            updateAdd(tree[x].rs, tree[x].add);

        tree[x].add = 0;
    }
}

int which(int x) {
    return (tree[tree[x].fa].rs == x);
}

bool isRoot(int x) {
    return (tree[tree[x].fa].ls != x && tree[tree[x].fa].rs != x);
}

void rotate(int x) {
    int y = tree[x].fa, z = tree[y].fa, k = which(x), w = tree[x][k ^ 1];

    if (!isRoot(y))
        tree[z][which(y)] = x;

    tree[x][k ^ 1] = y;
    tree[y][k] = w;
    tree[x].fa = z;
    tree[y].fa = x;

    if (w)
        tree[w].fa = y;

    pushup(y);
    pushup(x);
}

void splay(int x) {
    int top = 0;
    st[++top] = x;

    for (int y = x; !isRoot(y); y = tree[y].fa)
        st[++top] = tree[y].fa;

    while (top)
        pushdown(st[top--]);

    for (int y; y = tree[x].fa, !isRoot(x); rotate(x)) {
        if (!isRoot(y))
            rotate(which(x) == which(y) ? y : x);
    }
}

void access(int x) {
    for (int s = 0; x; s = x, x = tree[x].fa) {
        splay(x);
        tree[x].rs = s;
        pushup(x);
    }
}

void makeRoot(int x) {
    access(x);
    splay(x);
    updateReverse(x);
}

int findRoot(int x) {
    access(x);
    splay(x);
    pushdown(x);

    while (tree[x].ls) {
        x = tree[x].ls;
        pushdown(x);
    }

    splay(x);
    return x;
}

void link(int x, int y) {
    makeRoot(x);
    
    if (findRoot(y) != x)
        tree[x].fa = y;
}

void cut(int x, int y) {
    makeRoot(x);

    if (findRoot(y) == x && tree[y].fa == x && !tree[y].ls) {
        tree[y].fa = tree[x].rs = 0;
        pushup(x);
    }
}

int split(int x, int y) {
    makeRoot(x);
    access(y);
    splay(y);
    return y;
}

void init(int x) {
    tree[x].ls = tree[x].rs = tree[x].fa = 0;
    tree[x].size = 1;
    tree[x].rev = false;
    tree[x].key = tree[x].sum = 1;
    tree[x].add = 0;
    tree[x].mul = 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin >> n >> q;

    for (int i = 1; i <= n; ++i)
        init(i);

    for (int i = 1; i < n; ++i) {
        int x, y;
        cin >> x >> y;
        link(x, y);
    }

    while (q--) {
        char op[5];
        int x, y, a, b;
        cin >> op >> x >> y;

        if (op[0] == '+') {
            cin >> a;
            updateAdd(split(x, y), a);
        } else if (op[0] == '-') {
            cin >> a >> b;
            cut(x, y);
            link(a, b);
        } else if (op[0] == '*') {
            cin >> a;
            updateMultiply(split(x, y), a);
        } else {
            cout << tree[split(x, y)].sum << endl;
        }
    }

    return 0;
}