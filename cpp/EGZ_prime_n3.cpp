/**
 *    author:  riroan
 *    created:  2023.10.08 01:19:30
 **/
#include <bits/stdc++.h>
#pragma GCC optimize("O3")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#define endl '\n'
#define int long long
const int oOvOo = 0;
using ll = long long;
using namespace std;

signed main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n;
    cin >> n;
    int k = 2 * n - 1;
    vector<int> arr(k);
    for (auto &i : arr)
        cin >> i;
    vector<int> brr(k);
    vector<pair<int, int>> p(k);
    for (int i = 0; i < k; i++)
        p[i] = {arr[i] % k, i};
    sort(p.begin(), p.end());
    for (int i = 0; i < k; i++)
        brr[i] = p[i].first;
    vector<vector<int>> crr(n, vector<int>(k, -1));
    vector<int> v(n);
    int x = 0;
    for (int i = 0; i < n; i++)
    {
        x += brr[i];
        x %= n;
    }
    v[x] = 1;
    for (int i = 0; i < k; i++)
        crr[x][i] = (i < n);
    for (int i = 0; i < n - 1; i++)
    {
        int d = brr[i + n] - brr[i];
        for (int j = 0; j < n; j++)
        {
            if (!v[j])
                continue;
            if (v[(j + d) % n])
                continue;
            for (int l = 0; l < k; l++)
                crr[(j + d) % n][l] = crr[j][l];
            crr[(j + d) % n][i] = 0;
            crr[(j + d) % n][i + n] = 1;
            v[(j + d) % n] = 1;
        }
    }
    for (int i = 0; i < k; i++)
        if (crr[0][i])
            cout << arr[p[i].second] << " ";
    cout << endl;
    return oOvOo;
}
