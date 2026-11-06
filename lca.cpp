#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int LOG = 20; // Sufficient for N <= 1e6

vector<vector<int>> adj, up;
vector<int> depth;

void binaryLifting(int root, int n) {

    up.assign(n, vector<int>(LOG, root));
    depth.assign(n, 0);

    vector<int> parent(n, -1);
    parent[root] = root;

    vector<int> order = {root};

    for (int i = 0; i < (int)order.size(); i++) {
        int u = order[i];

        up[u][0] = parent[u];

        for (int j = 1; j < LOG; j++) {
            up[u][j] = up[up[u][j - 1]][j - 1];
        }

        for (int v : adj[u]) {
            if (v == parent[u]) continue;

            parent[v] = u;
            depth[v] = depth[u] + 1;
            order.push_back(v);
        }
    }
}

//0 indexed
int main() {
    int n;
    cin >> n;

    adj.assign(n,vector<int>());

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        --u; --v;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    auto lift = [&](int u, int k) -> int {
        if (k > depth[u]) return -1;

        for (int j = LOG - 1; j >= 0; j--) {
            if (k & (1LL << j)) {
                u = up[u][j];
            }
        }

        return u;
    };

    auto lca = [&](int u, int v) -> int {
        if (depth[u] < depth[v]) swap(u, v);

        u = lift(u, depth[u] - depth[v]);

        if (u == v) return u;

        for (int j = LOG - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        }

        return up[u][0];
    };

    binaryLifting(0, n);

    cout << lca(3, 5) << '\n';
    cout << lift(5, 2) << '\n';
}