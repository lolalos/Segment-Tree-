// gss1segmenttree.cpp
// Solución al problema GSS1 de SPOJ usando Segment Tree
// Problema: Encontrar la suma máxima de un subarreglo en un rango dado
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;
using ll = long long;

struct Node {
    ll sum;     // suma total del segmento
    ll pref;    // mejor suma de prefijo
    ll suf;     // mejor suma de sufijo
    ll best;    // mejor suma de subarreglo
    
    Node() : sum(0), pref(LLONG_MIN / 4), suf(LLONG_MIN / 4), best(LLONG_MIN / 4) {}
    Node(ll v) : sum(v), pref(v), suf(v), best(v) {}
};

Node mergeNode(const Node& L, const Node& R) {
    Node P;
    P.sum = L.sum + R.sum;
    P.pref = max(L.pref, L.sum + R.pref);
    P.suf = max(R.suf, R.sum + L.suf);
    P.best = max({L.best, R.best, L.suf + R.pref});
    return P;
}

struct SegmentTree {
    int n;
    vector<Node> tree;
    
    SegmentTree(const vector<ll>& arr) {
        n = (int)arr.size();
        tree.assign(4 * n, Node());
        build(1, 0, n - 1, arr);
    }
    
    void build(int pos, int left, int right, const vector<ll>& arr) {
        if (left == right) {
            tree[pos] = Node(arr[left]);
            return;
        }
        int mid = (left + right) / 2;
        build(pos * 2, left, mid, arr);
        build(pos * 2 + 1, mid + 1, right, arr);
        tree[pos] = mergeNode(tree[pos * 2], tree[pos * 2 + 1]);
    }
    
    Node queryNode(int pos, int left, int right, int ql, int qr) {
        if (ql > right || qr < left) return Node();
        if (ql <= left && right <= qr) return tree[pos];
        int mid = (left + right) / 2;
        if (qr <= mid) return queryNode(pos * 2, left, mid, ql, qr);
        if (ql > mid) return queryNode(pos * 2 + 1, mid + 1, right, ql, qr);
        Node L = queryNode(pos * 2, left, mid, ql, qr);
        Node R = queryNode(pos * 2 + 1, mid + 1, right, ql, qr);
        return mergeNode(L, R);
    }
    
    ll queryMax(int l, int r) {
        Node res = queryNode(1, 0, n - 1, l, r);
        return res.best;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<ll> arr(n);
    for (int i = 0; i < n; ++i) cin >> arr[i];

    SegmentTree seg(arr);

    int q;
    cin >> q;
    while (q--) {
        int l, r;
        cin >> l >> r; // entrada en SPOJ es 1-based
        l--; r--;      // convertir a 0-based
        cout << seg.queryMax(l, r) << '\n';
    }
    return 0;
}
