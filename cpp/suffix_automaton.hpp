#include <bits/stdc++.h>
using namespace std;

template <typename T>
class SegmentTree
{
public:
    int n;
    vector<T> arr;
    function<T(T, T)> func;
    T basis;
    SegmentTree()
    {
    }
    SegmentTree(vector<T> &brr, function<T(T, T)> f, T b)
    {
        n = brr.size();
        arr = vector<T>(n * 2);
        func = f;
        basis = b;
        init(brr, 0, n - 1, 1);
    }
    void init(vector<T> &brr, int left, int right, int node)
    {
        for (int i = 0; i < n; i++)
            arr[i + n] = brr[i];
        for (int i = n - 1; i > 0; --i)
            arr[i] = func(arr[i << 1], arr[i << 1 | 1]);
    }
    T query(int left, int right)
    {
        int res = basis;
        for (left += n, right += n + 1; left < right; left >>= 1, right >>= 1)
        {
            if (left & 1)
                res = func(res, arr[left++]);
            if (right & 1)
                res = func(res, arr[--right]);
        }
        return res;
    }

    void update(int p, T newValue)
    {
        for (arr[p += n] = newValue; p > 1; p >>= 1)
            arr[p >> 1] = func(arr[p], arr[p ^ 1]);
    }
};

struct State
{
    signed len, link;
    int cnt = 0LL, d = 0LL;
    map<char, signed> next;
    vector<int> inv_link;
};

struct SuffixAutomaton
{
    vector<State> st;
    set<int> terminals;
    SegmentTree<int> segtree;
    int sz, last, l;
    SuffixAutomaton()
    {
        l = 400001;
        init();
    }
    SuffixAutomaton(string s)
    {
        l = s.size() * 2 + 1;
        init();
        build(s);
        postprocessing();
    }
    void init()
    {
        st.resize(l);
        sz = 0;
        last = 0;
        st[0].len = 0;
        st[0].link = -1;
        sz++;
        vector<int> brr(l);
        segtree = SegmentTree<int>(
            brr, [](int a, int b)
            { return a + b; },
            0LL);
    }

    void postprocessing()
    {
        for (int i = 0; i < sz; i++)
            for (auto [x, y] : st[i].next)
                st[y].cnt += st[i].cnt + (i == 0);
        for (int i = 1; i < sz; i++)
            st[st[i].link].inv_link.push_back(i);
        get_d(0);
        st[0].d--;
    }

    int get_d(int ix)
    {
        if (st[ix].d > 0)
            return st[ix].d;
        int r = 1;
        for (auto [x, y] : st[ix].next)
            r += get_d(y);
        return st[ix].d = r;
    }

    void build(string s)
    {
        for (auto i : s)
            sa_extend(i);
        int p = last;
        while (p > 0)
        {
            terminals.insert(p);
            p = st[p].link;
        }
    }

    void update(int ix)
    {
        segtree.update(ix, st[ix].len - st[st[ix].link].len);
    }

    void sa_extend(char c)
    {
        int cur = sz++;
        st[cur].len = st[last].len + 1;
        int p = last;
        while (p != -1 && !st[p].next.count(c))
        {
            st[p].next[c] = cur;
            p = st[p].link;
        }
        if (p == -1)
        {
            st[cur].link = 0;
        }
        else
        {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len)
            {
                st[cur].link = q;
            }
            else
            {
                int clone = sz++;
                st[clone].len = st[p].len + 1;
                st[clone].next = st[q].next;
                st[clone].link = st[q].link;
                update(clone);
                while (p != -1 && st[p].next[c] == q)
                {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
                update(q);
            }
        }
        update(cur);
        last = cur;
    }

    int get_diff_strings()
    {
        return segtree.query(0, sz - 1);
    }

    int get_tot_len_diff_substrings()
    {
        int tot = 0;
        for (int i = 1; i < sz; i++)
        {
            int shortest = st[st[i].link].len + 1;
            int longest = st[i].len;

            int num_strings = longest - shortest + 1;
            int cur = num_strings * (longest + shortest) / 2;
            tot += cur;
        }
        return tot;
    }

    string get_lexicographically_kth_string(int k)
    {
        // TODO
        return "";
    }

    int go(string w)
    {
        int cur = 0;
        for (auto i : w)
        {
            cur = st[cur].next[i];
            if (cur == 0)
                return 0;
        }
        return cur;
    }

    bool is_substring(string w)
    {
        return go(w) > 0;
    }

    bool is_suffix(string w)
    {
        return terminals.contains(go(w));
    }

    int count(string w)
    {
        // TODO
        return 1;
    }

    string lcs(string w)
    {
        int v = 0, l = 0, best = 0, bestpos = 0;
        for (int i = 0; i < w.size(); i++)
        {
            while (v && !st[v].next.count(w[i]))
            {
                v = st[v].link;
                l = st[v].len;
            }
            if (st[v].next.count(w[i]))
            {
                v = st[v].next[w[i]];
                l++;
            }
            if (l > best)
            {
                best = l;
                bestpos = i;
            }
        }
        return w.substr(bestpos - best + 1, best);
    }
};
