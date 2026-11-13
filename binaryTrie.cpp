
#include <bits/stdc++.h>
using namespace std;

/*
    BINARY TRIE (INT VERSION)

    Supports:
    1. insert(x)    - Insert a number
    2. search(x)    - Check if a number exists
    3. remove(x)    - Remove one occurrence
    4. max_xor(x)   - Maximum XOR with an existing number
    5. size()       - Total number of elements

    Supports non-negative signed int values.
    Bits processed: 30 to 0.
*/

struct BinaryTrie {

    static const int MAX_BITS = 30;

    struct Node {
        int child[2] = {0, 0};
        int cnt = 0;
    };

    vector<Node> trie;

    // Creates the root node.
    BinaryTrie() {
        trie.emplace_back();
    }

    // Returns the total number of elements.
    int size() {
        return trie[0].cnt;
    }

    // Checks whether the Trie is empty.
    bool empty() {
        return size() == 0;
    }

    // Inserts x into the Trie.
    void insert(int x) {
        int cur = 0;
        trie[cur].cnt++;

        for (int i = MAX_BITS; i >= 0; i--) {
            int b = (x >> i) & 1;
            int nxt = trie[cur].child[b];

            if (nxt == 0) {
                nxt = (int)trie.size();
                trie.emplace_back();
                trie[cur].child[b] = nxt;
            }

            cur = nxt;
            trie[cur].cnt++;
        }
    }

    // Returns true if x exists in the Trie.
    bool search(int x) {
        int cur = 0;

        for (int i = MAX_BITS; i >= 0; i--) {
            int b = (x >> i) & 1;
            cur = trie[cur].child[b];

            if (cur == 0)
                return false;
        }

        return trie[cur].cnt > 0;
    }

    // Removes one occurrence of x.
    // Returns false if x does not exist.
    bool remove(int x) {
        if (!search(x))
            return false;

        int cur = 0;
        trie[cur].cnt--;

        for (int i = MAX_BITS; i >= 0; i--) {
            int b = (x >> i) & 1;
            cur = trie[cur].child[b];
            trie[cur].cnt--;
        }

        return true;
    }

    // Returns maximum XOR of x with any number in the Trie.
    // Call only when the Trie is non-empty.
    int max_xor(int x) {
        int cur = 0;
        int ans = 0;

        for (int i = MAX_BITS; i >= 0; i--) {
            int b = (x >> i) & 1;
            int want = b ^ 1;

            int nxt = trie[cur].child[want];

            if (nxt != 0 && trie[nxt].cnt > 0) {
                cur = nxt;
                ans |= (1 << i);
            }
            else {
                cur = trie[cur].child[b];
            }
        }

        return ans;
    }
};


// ---------------- USAGE ----------------

int main() {

    BinaryTrie bt;

    bt.insert(5);
    bt.insert(10);
    bt.insert(15);
    bt.insert(5);       // Duplicates allowed

    cout << bt.size() << '\n';       // 4

    cout << bt.search(10) << '\n';   // 1
    cout << bt.search(20) << '\n';   // 0

    cout << bt.max_xor(5) << '\n';   // 15

    bt.remove(5);                    // Removes one occurrence

    cout << bt.search(5) << '\n';    // 1 (another 5 remains)

    bt.remove(5);

    cout << bt.search(5) << '\n';    // 0

    cout << bt.size() << '\n';       // 2

    return 0;
}
