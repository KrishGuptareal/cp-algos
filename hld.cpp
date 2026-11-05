
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// =====================================================
//                  SEGMENT TREE
// =====================================================
/*
    T can be any data type just remember that there should be some way to merge and identity
*/
template <typename T, class Merge>
class SegmentTree {
    int n, S;
    T ID;
    Merge merge;

    vector<T> seg, rev;

public:

    SegmentTree(int n, T identity, Merge op = Merge())
        : n(n), ID(identity), merge(op) {}

    // Build from a 1-indexed array
    void build(const vector<T>& a) {
        S = 1;
        while (S < n) S <<= 1;

        seg.assign(2 * S, ID);
        rev.assign(2 * S, ID);

        for (int i = 1; i <= n; i++) {
            seg[S + i - 1] = a[i];
            rev[S + i - 1] = a[i];
        }

        for (int i = S - 1; i >= 1; i--) {
            // Normal order: left -> right
            seg[i] = merge(seg[2 * i], seg[2 * i + 1]);

            // Reverse order: right -> left
            rev[i] = merge(rev[2 * i + 1], rev[2 * i]);
        }
    }

    // Point update: set index idx to value
    void update(int idx, T value) {
        int p = S + idx - 1;

        seg[p] = value;
        rev[p] = value;

        for (p >>= 1; p > 0; p >>= 1) {
            seg[p] = merge(seg[2 * p], seg[2 * p + 1]);
            rev[p] = merge(rev[2 * p + 1], rev[2 * p]);
        }
    }

    // Range query: l -> r
    T query(int l, int r) {
        T leftAns = ID;
        T rightAns = ID;

        l += S - 1;
        r += S - 1;

        while (l <= r) {
            if (l & 1)
                leftAns = merge(leftAns, seg[l++]);

            if (!(r & 1))
                rightAns = merge(seg[r--], rightAns);

            l >>= 1;
            r >>= 1;
        }

        return merge(leftAns, rightAns);
    }

    // Range query: r -> l
    T queryReverse(int l, int r) {
        T leftAns = ID;
        T rightAns = ID;

        l += S - 1;
        r += S - 1;

        while (l <= r) {
            if (l & 1)
                leftAns = merge(rev[l++], leftAns);

            if (!(r & 1))
                rightAns = merge(rightAns, rev[r--]);

            l >>= 1;
            r >>= 1;
        }

        return merge(rightAns, leftAns);
    }
};

template <typename T, class Merge>
class HLD {
    int n, timer = 0;

    vector<vector<int>> adj;

    vector<int> sz, parent, depth;
    vector<int> heavy, head, pos;

    vector<T> val, base;

    T ID;
    Merge merge;

    SegmentTree<T, Merge> seg;

    // -------------------------------------------------
    // Iterative HLD preprocessing
    // -------------------------------------------------

    void buildHLD(int root) {
        vector<int> order;
        order.reserve(n);

        vector<int> st;
        st.push_back(root);

        parent[root] = 0;

        // Step 1: Find parent, depth and traversal order
        while (!st.empty()) {
            int u = st.back();
            st.pop_back();

            order.push_back(u);

            for (int v : adj[u]) {
                if (v == parent[u]) continue;

                parent[v] = u;
                depth[v] = depth[u] + 1;

                st.push_back(v);
            }
        }

        // Step 2: Calculate subtree sizes and heavy child
        for (int i = n - 1; i >= 0; i--) {
            int u = order[i];

            sz[u] = 1;
            heavy[u] = -1;

            int mx = 0;

            for (int v : adj[u]) {
                if (parent[v] != u) continue;

                sz[u] += sz[v];

                if (sz[v] > mx) {
                    mx = sz[v];
                    heavy[u] = v;
                }
            }
        }

        // Step 3: Decompose into heavy chains
        vector<pair<int, int>> chains;
        chains.push_back({root, root});

        timer = 0;

        while (!chains.empty()) {
            auto [start, h] = chains.back();
            chains.pop_back();

            int u = start;

            // Process the complete heavy chain
            while (u != -1) {
                head[u] = h;
                pos[u] = ++timer;

                base[timer] = val[u];

                // Add light children as new chains
                for (int v : adj[u]) {
                    if (parent[v] == u && v != heavy[u]) {
                        chains.push_back({v, v});
                    }
                }

                u = heavy[u];
            }
        }
    }

public:

    // -------------------------------------------------
    // Constructor
    // -------------------------------------------------

    HLD(int n, T identity, Merge op = Merge())
        : n(n),
          adj(n + 1),
          sz(n + 1),
          parent(n + 1),
          depth(n + 1),
          heavy(n + 1),
          head(n + 1),
          pos(n + 1),
          val(n + 1),
          base(n + 1),
          ID(identity),
          merge(op),
          seg(n, identity, op) {}

    // -------------------------------------------------
    // Input
    // -------------------------------------------------

    void setValue(int u, T v) {
        val[u] = v;
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // -------------------------------------------------
    // Build
    // -------------------------------------------------

    void build(int root = 1) {
        buildHLD(root);
        seg.build(base);
    }

    // -------------------------------------------------
    // Point update
    // -------------------------------------------------

    void update(int u, T v) {
        val[u] = v;
        seg.update(pos[u], v);
    }

    // -------------------------------------------------
    // Path query: u -> v
    // -------------------------------------------------

    T query(int u, int v) {
        T leftAns = ID;
        T rightAns = ID;

        while (head[u] != head[v]) {

            if (depth[head[u]] >= depth[head[v]]) {

                // u -> head[u] (reverse direction)
                T cur = seg.queryReverse(
                    pos[head[u]], pos[u]
                );

                leftAns = merge(leftAns, cur);

                u = parent[head[u]];

            }
            else {

                // head[v] -> v (forward direction)
                T cur = seg.query(
                    pos[head[v]], pos[v]
                );

                rightAns = merge(cur, rightAns);

                v = parent[head[v]];
            }
        }

        // Both nodes are now in the same chain

        if (depth[u] >= depth[v]) {

            T cur = seg.queryReverse(
                pos[v], pos[u]
            );

            leftAns = merge(leftAns, cur);

        }
        else {

            T cur = seg.query(
                pos[u], pos[v]
            );

            leftAns = merge(leftAns, cur);
        }

        return merge(leftAns, rightAns);
    }
};


// =====================================================
//                CUSTOM MERGE OPERATIONS
// =====================================================

// Maximum
struct MaxMerge {
    ll operator()(ll a, ll b) const {
        return max(a, b);
    }
};

// Sum
struct SumMerge {
    ll operator()(ll a, ll b) const {
        return a + b;
    }
};

// Minimum
struct MinMerge {
    ll operator()(ll a, ll b) const {
        return min(a, b);
    }
};

// XOR
struct XorMerge {
    ll operator()(ll a, ll b) const {
        return a ^ b;
    }
};

// GCD
struct GcdMerge {
    ll operator()(ll a, ll b) const {
        return gcd(a, b);
    }
};


// =====================================================
//                       MAIN
// =====================================================

void solve() {
    int n, q;
    cin >> n >> q;

    // For PATH MAXIMUM:
    //hld<T,mergestruct> hld(n,identity)
    /*
        Identity is a value that doesn't change the result when you merge it with another value.
        
        merge(a, identity) = a

        Examples
        Operation	Merge	Identity
        Sum	a + b	0
        Multiplication	a * b	1
        Maximum	max(a,b)	LLONG_MIN
        Minimum	min(a,b)	LLONG_MAX
        XOR	a ^ b	0
        GCD	gcd(a,b)	0
    */
    HLD<ll, MaxMerge> hld(n, LLONG_MIN);

    for (int i = 1; i <= n; i++) {
        ll x;
        cin >> x;
        hld.setValue(i, x);
    }

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        hld.addEdge(u, v);
    }

    hld.build();

    while (q--) {
        int type, u;
        cin >> type >> u;

        if (type == 1) {
            ll x;
            cin >> x;
            hld.update(u, x);
        }
        else {
            int v;
            cin >> v;
            cout << hld.query(u, v) << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();

    return 0;
}
