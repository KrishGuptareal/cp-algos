#include <bits/stdc++.h>
using namespace std;
using ll=long long;
//this is 0 indexed
// ==========================================
// LAZY SEGMENT TREE
// ==========================================

struct SumOp {
    ll operator()(ll a, ll b) const {
        return a + b;
    }
};

// Apply an addition tag to a segment.
struct RangeAdd {
    ll operator()(ll tag, ll sum, int len) const {
        return sum + tag * len;
    }
};

// Combine two addition tags.
struct AddComposition {
    ll operator()(ll newer, ll older) const {
        return newer + older;
    }
};

template<class S, class F, class Op, class Mapping, class Composition>
class LazySegTree {
private:
    int n;

    vector<S> st;
    vector<F> lazy;
    vector<char> hasLazy;

    S ID;
    F ID_TAG;

    Op op;
    Mapping mapping;
    Composition composition;

    // Apply a new tag to a node.
    void applyNode(int p, int l, int r, F tag) {

        // Update node value.
        // Mapping receives:
        // tag, current node value, segment length.
        st[p] = mapping(tag, st[p], r - l + 1);

        // Combine new tag with previously pending tag.
        // composition(newTag, oldTag).
        if (hasLazy[p]) {
            lazy[p] = composition(tag, lazy[p]);
        } else {
            lazy[p] = tag;
        }

        hasLazy[p] = true;
    }

    // Push pending update to children.
    void push(int p, int l, int r) {
        if (!hasLazy[p] || l == r)
            return;

        int mid = (l + r) / 2;

        applyNode(2 * p, l, mid, lazy[p]);
        applyNode(2 * p + 1, mid + 1, r, lazy[p]);

        lazy[p] = ID_TAG;
        hasLazy[p] = false;
    }

    void pull(int p) {
        st[p] = op(st[2 * p], st[2 * p + 1]);
    }

    void build(int p, int l, int r, const vector<S>& a) {
        if (l == r) {
            st[p] = a[l];
            return;
        }

        int mid = (l + r) / 2;

        build(2 * p, l, mid, a);
        build(2 * p + 1, mid + 1, r, a);

        pull(p);
    }

    void update(int p, int l, int r,
                int ql, int qr, F tag) {

        if (qr < l || r < ql)
            return;

        if (ql <= l && r <= qr) {
            applyNode(p, l, r, tag);
            return;
        }

        push(p, l, r);

        int mid = (l + r) / 2;

        update(2 * p, l, mid, ql, qr, tag);
        update(2 * p + 1, mid + 1, r, ql, qr, tag);

        pull(p);
    }

    S query(int p, int l, int r, int ql, int qr) {

        if (qr < l || r < ql)
            return ID;

        if (ql <= l && r <= qr)
            return st[p];

        push(p, l, r);

        int mid = (l + r) / 2;

        return op(
            query(2 * p, l, mid, ql, qr),
            query(2 * p + 1, mid + 1, r, ql, qr)
        );
    }

    void pointSet(int p, int l, int r, int idx, S val) {
        if (l == r) {
            st[p] = val;
            lazy[p] = ID_TAG;
            hasLazy[p] = false;
            return;
        }

        push(p, l, r);

        int mid = (l + r) / 2;

        if (idx <= mid)
            pointSet(2 * p, l, mid, idx, val);
        else
            pointSet(2 * p + 1, mid + 1, r, idx, val);

        pull(p);
    }

public:
    LazySegTree(int n, S ID, F ID_TAG,
                Op op = Op(),
                Mapping mapping = Mapping(),
                Composition composition = Composition())
        : n(n), ID(ID), ID_TAG(ID_TAG),
          op(op), mapping(mapping), composition(composition) {

        int sz = 4 * max(1, n);

        st.assign(sz, ID);
        lazy.assign(sz, ID_TAG);
        hasLazy.assign(sz, false);
    }

    LazySegTree(const vector<S>& a, S ID, F ID_TAG,
                Op op = Op(),
                Mapping mapping = Mapping(),
                Composition composition = Composition())
        : LazySegTree((int)a.size(), ID, ID_TAG,
                      op, mapping, composition) {
        build(a);
    }

    void build(const vector<S>& a) {

        fill(lazy.begin(), lazy.end(), ID_TAG);
        fill(hasLazy.begin(), hasLazy.end(), false);

        if (n > 0)
            build(1, 0, n - 1, a);
    }

    // Apply tag to range [l, r].
    void rangeApply(int l, int r, F tag) {
        update(1, 0, n - 1, l, r, tag);
    }

    // Query range [l, r].
    S rangeQuery(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }

    // Point assignment: a[idx] = val.
    void update(int idx, S val) {
        pointSet(1, 0, n - 1, idx, val);
    }

    S allQuery() const {
        return n ? st[1] : ID;
    }
};

int main() {
    vector<ll> a = {1, 2, 3, 4, 5};

    LazySegTree<
        ll,
        ll,
        SumOp,
        RangeAdd,
        AddComposition
    > st(a, 0LL, 0LL);

    cout << st.rangeQuery(0, 4) << '\n';
    // 15

    // Add 10 to all elements in [1, 3].
    st.rangeApply(1, 3, 10);

    // Array: {1, 12, 13, 14, 5}

    cout << st.rangeQuery(0, 4) << '\n';
    // 45

    cout << st.rangeQuery(1, 3) << '\n';
    // 39

    // Point assignment.
    st.update(2, 100);

    // Array: {1, 12, 100, 14, 5}

    cout << st.rangeQuery(0, 4) << '\n';
    // 132
}
