#include <bits/stdc++.h>
using namespace std;
#define ll long long  
#define pb push_back
#define vi vector<int>
#define vll vector<ll>

//problem : cses fixed length 1

/*
CENTROID DECOMPOSITION NOTES:

1. Always calculate subtree sizes BEFORE finding centroid.
2. Always ignore removed[] nodes in every DFS.
3. After removing centroid, recurse on adj[centroid],
   NOT adj[original_node].

Complexity:
- Decomposition depth: O(log N)
- Total component traversals: O(N log N)
- Actual complexity depends on process_centroid().

For pair counting:
- Query before update to avoid same-subtree pairs.
- Paths not passing through current centroid are
  handled at lower decomposition levels.

Stack:
- Recursive DFS can overflow on a chain of N nodes.
  Use iterative traversals if necessary.
*/

/*-----------------------solution starts here---------*/
int n;
vector<vi> adj;
vector<bool> removed;
vi subtree_size;
int k;
ll answer=0;
vi all_distances;
void memassign(){
    all_distances.assign(k+1,0);
    adj.assign(n,vi());
    removed.assign(n,false);
    subtree_size.assign(n,0);
}
//generic
int get_subtree_size(int node,int par){
    subtree_size[node]=1;
    for(int i:adj[node]){
        if(removed[i]||par==i)continue;
        subtree_size[node]+=get_subtree_size(i,node);
    }
    return subtree_size[node];
}
//generic
int get_centriod(int node,int par,int tree_size){
    for(int i:adj[node]){
        if(par==i||removed[i])continue;
        if(2*subtree_size[i]>tree_size)
            return get_centriod(i,node,tree_size);
    }
    return node;
}
//problem specific
void get_distances(int node,int par,int dist,vi &distances){
    if(dist>k)return;
    distances.pb(dist);
    for(int i:adj[node]){
        if(i==par||removed[i])continue;
        get_distances(i,node,dist+1,distances);
    }
}
//problem specific
void process_centriod(int centriod){
    vi touched;
    touched.pb(0);
    //we will calculate all the paths passing through this centriod only
    all_distances[0]=1;
    for(int i:adj[centriod]){
        if(removed[i])continue;
        vi distances;
        get_distances(i,centriod,1,distances);
        for(int d:distances){
            answer+=all_distances[k-d];
        }
        for(int d : distances){
            if(all_distances[d] == 0)
                touched.pb(d);
            all_distances[d]++;
        }
    }
    for(int i:touched)all_distances[i]=0;
}
// Generic centroid decomposition.
//
// 1. Calculate size of current component.
// 2. Find its centroid.
// 3. Process all paths passing through this centroid.
// 4. Remove centroid.
// 5. Recursively decompose each remaining component.
//
// Each recursive component has at most half the previous size,
// so decomposition depth is O(log n).
void decompose(int v){
    int tree_size=get_subtree_size(v,-1);
    int centriod=get_centriod(v,-1,tree_size);
    process_centriod(centriod);
    removed[centriod]=1;
    for(int i:adj[centriod]){
        if(removed[i])continue;
        decompose(i);
    }
}
void solve(){
    answer=0;
    cin>>n>>k;
    memassign();
    for(int i=1;i<n;i++){
        int a,b;cin>>a>>b;a--;b--;
        adj[a].pb(b);adj[b].pb(a);
    }
    decompose(0);
    cout<<answer<<endl;
}
int main(){
 ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin>>t;

    while(t--) {
        
        solve();
    }

    return 0;
    
}
