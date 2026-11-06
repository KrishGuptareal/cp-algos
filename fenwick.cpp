
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

/*
    FENWICK TREE GUIDE(1 indexed)

    1-indexed data structure.

    Constructor:
        Fenwick<ll, Add> fw(n, 0LL);
        Fenwick<ll, MinOp> fw(n, LLONG_MAX);
        Fenwick<ll, MaxOp> fw(n, LLONG_MIN);

    update(i, val):
        Sum: Add val to position i.
        Min: Apply chmin at position i.
        Max: Apply chmax at position i.

    query(i):
        Returns aggregate over [1, i].

    Range SUM [l, r]:
        query(r) - query(l - 1).

    build(a):
        O(n), expects a to be 1-indexed.

    Complexity:
        Update: O(log n)
        Query:  O(log n)
        Build:  O(n)

    IMPORTANT:
        Min supports decreasing updates only.
        Max supports increasing updates only.
        Arbitrary point assignment for min/max:
        Use Segment Tree instead.
*/
// Operations
struct Add {
    ll operator()(ll a, ll b) const {
        return a + b;
    }
};

struct MinOp {
    ll operator()(ll a, ll b) const {
        return min(a, b);
    }
};

struct MaxOp {
    ll operator()(ll a, ll b) const {
        return max(a, b);
    }
};

template<class T, class Op>
class Fenwick {
private:
    int n;
    vector<T> bit;
    T ID;
    Op op;

public:
    // ID = identity element of the operation
    Fenwick(int n, T ID) : n(n), bit(n + 1, ID), ID(ID) {}

    // Applies val at position i.
    // For sum: adds val.
    // For min: takes min with val.
    // For max: takes max with val.
    void update(int i, T val) {
        while (i <= n) {
            bit[i] = op(bit[i], val);
            i += i & -i;
        }
    }

    // Returns aggregate over prefix [1, i].
    T query(int i) const {
        T res = ID;

        while (i > 0) {
            res = op(res, bit[i]);
            i -= i & -i;
        }

        return res;
    }

    // O(n) build from a 1-indexed array.
    void build(const vector<T>& a) {

        for (int i = 1; i <= n; i++) {
            bit[i] = a[i];
        }

        for (int i = 1; i <= n; i++) {
            int p = i + (i & -i);

            if (p <= n) {
                bit[p] = op(bit[p], bit[i]);
            }
        }
    }
};


int main() {
    int n = 5;

    // --------------------------------
    // 1. SUM FENWICK TREE
    // --------------------------------

    Fenwick<ll, Add> fw(n, 0LL);

    fw.update(1, 5);
    fw.update(3, 2);
    fw.update(5, 7);

    // Prefix sum [1, 4]
    cout << fw.query(4) << '\n';

    // Range sum [2, 5]
    // Only works because addition has an inverse.
    cout << fw.query(5) - fw.query(1) << '\n';


    // --------------------------------
    // 2. MIN FENWICK TREE
    // --------------------------------

    Fenwick<ll, MinOp> mn(n, LLONG_MAX);

    mn.update(2, 8);
    mn.update(4, 3);
    mn.update(3, 6);

    // Minimum over [1, 4]
    cout << mn.query(4) << '\n';

    // Decrease an existing value
    mn.update(2, 1);

    cout << mn.query(4) << '\n';


    // --------------------------------
    // 3. MAX FENWICK TREE
    // --------------------------------

    Fenwick<ll, MaxOp> mx(n, LLONG_MIN);

    mx.update(2, 8);
    mx.update(4, 3);
    mx.update(3, 6);

    // Maximum over [1, 4]
    cout << mx.query(4) << '\n';

    // Increase an existing value
    mx.update(2, 10);

    cout << mx.query(4) << '\n';
}
