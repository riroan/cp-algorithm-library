#include<bits/stdc++.h>
using namespace std;

template <typename T>
class SegmentTree
{
public:
    int n;
    vector<T> arr;
    function<T(T, T)> func;
    T basis;
    SegmentTree() {}
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

struct HLD
{
    vector<int> sz, d, p, top, in, out;
    vector<vector<int>> arr;
    SegmentTree<int> st;
    map<pair<int, int>, int> edges;
    int n, pv = 0;

    HLD(int n)
    {
        this->n = n;
        sz.resize(n);
        d.resize(n);
        p.resize(n);
        top.resize(n);
        in.resize(n);
        out.resize(n);
        arr.resize(n);
    }

    void add(int u, int v, int w = 1)
    {
        arr[u].push_back(v);
        arr[v].push_back(u);
        edges[minmax(u, v)] = w;
    }

    void make(int root = 0)
    {
        top[root] = root;
        d[root] = 0;
        p[root] = -1;
        dfs1(root);
        dfs2(root);
        vector<int> brr(n);
        for (int i = 0; i < n; i++)
            for (auto j : arr[i])
                brr[in[j]] = edges[minmax(i, j)];
        st = SegmentTree<int>(
            brr, [](int a, int b)
            { return a + b; },
            0LL);
    }

    void dfs1(int x = 0)
    {
        if (p[x] != -1)
            arr[x].erase(find(arr[x].begin(), arr[x].end(), p[x]));

        sz[x] = 1;
        for (auto &i : arr[x])
        {
            d[i] = d[x] + 1;
            p[i] = x;
            dfs1(i);
            sz[x] += sz[i];
            if (sz[i] > sz[arr[x][0]])
                swap(i, arr[x][0]);
        }
    }

    void dfs2(int x = 0)
    {
        in[x] = pv++;
        for (auto i : arr[x])
        {
            top[i] = i == arr[x][0] ? top[x] : i;
            dfs2(i);
        }
        out[x] = pv;
    }

    auto lca(int a, int b)
    {
        int ret = 0;
        while (top[a] ^ top[b])
        {
            if (d[top[a]] > d[top[b]])
                a = p[top[a]];
            else
                b = p[top[b]];
        }
        return d[a] < d[b] ? a : b;
    }

    auto unit_dist(int a, int b)
    {
        return d[a] + d[b] - 2 * d[lca(a, b)];
    }

    auto dist(int a, int b)
    {
        int ret = 0;
        while (top[a] != top[b])
        {
            if (d[top[a]] < d[top[b]])
                swap(a, b);
            ret += st.query(in[top[a]], in[a]);
            a = p[top[a]];
        }
        if (d[a] > d[b])
            swap(a, b);
        ret += st.query(in[a] + 1, in[b]);
        return ret;
    }

    auto query(int a, int b){
        // do something
    }

    bool isAncestor(int a, int b)
    {
        return in[a] <= in[b] && in[b] < out[a];
    }

    auto rootedLCA(int a, int b, int c)
    {
        return lca(a, b) ^ lca(b, c) ^ lca(c, a);
    }
};
