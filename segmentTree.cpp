#include <bits/stdc++.h>
using namespace std;
using ll=long long;

//this is 0 indexed

// ==========================================
// SEGMENT TREE (WITHOUT LAZY PROPAGATION)
// ==========================================

// Example operations
struct SumOp {
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
class SegTree {
private:
    int n;
    vector<T> st;
    T ID;
    Op op;

    void build(int p, int l, int r, const vector<T>& a) {
        if (l == r) {
            st[p] = a[l];
            return;
        }

        int mid = (l + r) / 2;

        build(2 * p, l, mid, a);
        build(2 * p + 1, mid + 1, r, a);

        st[p] = op(st[2 * p], st[2 * p + 1]);
    }

    void update(int p, int l, int r, int idx, T val) {
        if (l == r) {
            st[p] = val;
            return;
        }

        int mid = (l + r) / 2;

        if (idx <= mid)
            update(2 * p, l, mid, idx, val);
        else
            update(2 * p + 1, mid + 1, r, idx, val);

        st[p] = op(st[2 * p], st[2 * p + 1]);
    }

    T query(int p, int l, int r, int ql, int qr) const {
        if (r < ql || l > qr)
            return ID;

        if (ql <= l && r <= qr)
            return st[p];

        int mid = (l + r) / 2;

        return op(
            query(2 * p, l, mid, ql, qr),
            query(2 * p + 1, mid + 1, r, ql, qr)
        );
    }

public:
    // Initialize with n elements, all having identity value.
    SegTree(int n, T ID, Op op = Op())
        : n(n), ID(ID), op(op) {
        st.assign(4 * max(1, n), ID);
    }

    // Initialize and build from an array.
    SegTree(const vector<T>& a, T ID, Op op = Op())
        : SegTree((int)a.size(), ID, op) {
        build(a);
    }

    void build(const vector<T>& a) {
        if (n > 0)
            build(1, 0, n - 1, a);
    }

    // Point assignment: a[idx] = val
    void update(int idx, T val) {
        update(1, 0, n - 1, idx, val);
    }

    // Range query [l, r], inclusive.
    T query(int l, int r) const {
        return query(1, 0, n - 1, l, r);
    }

    // Query the entire array.
    T allQuery() const {
        return n ? st[1] : ID;
    }
};


struct GcdOp {
    ll operator()(ll a, ll b) const {
        return gcd(a, b);
    }
};

int main() {
    vector<ll> a = {5, 2, 8, 1, 9};

    // SUM
    SegTree<ll, SumOp> st1(a, 0LL);

    cout << st1.query(1, 3) << '\n'; // 11

    st1.update(2, 10); // a[2] = 10

    cout << st1.query(1, 3) << '\n'; // 13


    // MIN
    SegTree<ll, MinOp> st2(a, LLONG_MAX);

    cout << st2.query(0, 4) << '\n'; // 1


    // MAX
    SegTree<ll, MaxOp> st3(a, LLONG_MIN);

    cout << st3.query(0, 4) << '\n'; // 9


    // GCD
    SegTree<ll, GcdOp> st4(a, 0LL);

    cout << st4.query(0, 4) << '\n'; // 1
}

