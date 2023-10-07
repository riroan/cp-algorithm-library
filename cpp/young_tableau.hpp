#include <bits/stdc++.h>
using namespace std;

struct YoungTableau
{
    vector<vector<int>> arr;
    vector<pair<int, int>> seq;
    auto add(int x)
    {
        int sz = arr.size();
        bool end = false;
        int rx = -1, ry = -1;
        for (int i = 0; i < sz; i++)
        {
            int ix = upper_bound(arr[i].begin(), arr[i].end(), x) - arr[i].begin();
            if (ix == arr[i].size())
            {
                rx = i;
                ry = arr[i].size();
                arr[i].push_back(x);
                end = true;
                break;
            }
            swap(x, arr[i][ix]);
        }
        if (!end)
        {
            rx = arr.size();
            ry = 0;
            arr.push_back({x});
        }
        auto res = make_pair(rx, ry);
        seq.push_back(res);
        return res;
    }
    int remove(int s, int t)
    {
        int sz = arr.size();
        // check corner
        assert(s < sz && t == arr[s].size() - 1);
        if (s < sz - 1)
            assert(arr[s + 1].size() <= t);
        int x = arr[s][t];
        arr[s].pop_back();
        if (arr[s].size() == 0)
            arr.pop_back();
        for (int i = s - 1; i >= 0; i--)
        {
            int ix = upper_bound(arr[i].begin(), arr[i].end(), x) - arr[i].begin();
            ix--;
            swap(arr[i][ix], x);
        }
        return x;
    }
    void print()
    {
        for (auto i : arr)
        {
            for (auto j : i)
                cout << j << " ";
            cout << endl;
        }
    }
    auto mapping()
    {
        int sz = arr.size();
        vector<vector<int>> brr(sz);
        for (int i = 0; i < sz; i++)
            brr[i].resize(arr[i].size());
        for (int i = 0; i < seq.size(); i++)
        {
            auto [x, y] = seq[i];
            brr[x][y] = i + 1;
        }
        return brr;
    }
};
