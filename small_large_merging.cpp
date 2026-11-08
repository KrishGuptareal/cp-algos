#include <bits/stdc++.h>
using namespace std;
#define ll long long  
#define pb push_back
#define vi vector<int>
#define vll vector<ll>
 
 //problem cses fixed length 1

 /*
SMALL TO LARGE MERGING (DSU / SACK-STYLE MERGING)

CORE IDEA:
- Each subtree maintains some information in a data structure.
- When combining children, always merge the smaller data
  structure into the larger one.
- Swap the structures if necessary to ensure this property.
GENERAL PROCESS:
1. Recursively process children.
2. Obtain the data structures representing their subtrees.
3. Keep the largest structure as the base.
4. Merge smaller structures into the largest one.
5. Add/process the current node's information.
6. Return the merged structure to the parent.
IMPORTANT:
- The data structure and merge operation depend on the problem.
- Querying before merging may be necessary to avoid counting
  elements from the same subtree.
- Some problems require updating answers during merging.
- The small-to-large trick optimizes merging; it does NOT
  determine what information must be stored.
- Recursive DFS may cause stack overflow on a chain-shaped tree.
COMMON APPLICATIONS:
- Subtree color frequency queries.
- Number of distinct elements in each subtree.
- Merging maps / sets / frequency arrays.
- Counting pairs while combining subtrees.

*/

/*-----------------------solution starts here---------*/
int n;
vector<vi> adj;
int k;
ll answer=0;
void memassign(){
    adj.assign(n,vi());
}
deque<ll> dfs(int node,int par,int depth){
    deque<ll> res{1};//this is relative to the subtree
    for(int i:adj[node]){
        if(i==par)continue;
        auto child_dist=dfs(i,node,depth+1);
        if(child_dist.size()>res.size())swap(child_dist,res);
        int sz=child_dist.size();
        for(int i=0;i<sz;i++){
            if(k-i>=0&&k-i<(res.size()))
                answer+=1ll*res[k-i]*child_dist[i];
        }
        for(int i=0;i<sz;i++)res[i]+=child_dist[i];
    }
    res.push_front(0);
    return res;
}
void solve(){
    cin>>n>>k;
    memassign();
    for(int i=0;i<n-1;i++){
        int a,b;cin>>a>>b;
        a--;b--;
        adj[a].pb(b);
        adj[b].pb(a);
    }
    dfs(0,-1,0);
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
