mod = 10**9+7


def berlekamp_massey(x):
    if len(x) <= 1:
        return []
    a, b = x[:2]
    x = [i % mod for i in x]
    f, cur, d = 1, [1], [0]
    if a != b:
        cur, d = [b * pow(a, mod - 2, mod)], [1]

    def get(c, ix):
        res = 0
        for i in range(len(c)):
            res = (res + c[i] * x[ix - i]) % mod
        return res

    for i in range(2, len(x)):
        t = get(cur, i - 1)
        if t == x[i]:
            continue
        delta = (x[i] - t) % mod
        d = [1] + [mod - i for i in d]
        mul = delta * pow(get(d, f), mod - 2, mod) % mod
        d = [0] * (i - f - 1) + [(j * mul) % mod for j in d]
        for j in range(len(cur)):
            d[j] = (d[j] + cur[j]) % mod
        cur, d, f = d, cur, i

    return cur


def get_nth(rec, dp, n):
    m = len(rec)
    s, t = [0] * m, [0] * m
    s[0] = 1
    if m != 1:
        t[1] = 1
    else:
        t[0] = rec[0]

    def mul(v, w, rec):
        m = len(v)
        t = [0] * (2 * m)
        for j in range(m):
            for k in range(m):
                t[j + k] += v[j] * w[k] % mod
                if t[j + k] >= mod:
                    t[j + k] -= mod
        for j in range(2 * m - 1, m - 1, -1):
            for k in range(1, m + 1):
                t[j - k] += t[j] * rec[k - 1] % mod
                if t[j - k] >= mod:
                    t[j - k] -= mod
        t = t[:m]
        return t

    while n:
        if n & 1:
            s = mul(s, t, rec)
        t = mul(t, t, rec)
        n >>= 1
    ret = 0
    for i in range(m):
        ret += s[i] * dp[i] % mod
    return ret % mod


def guess_nth_term(x, n):
    if n < len(x):
        return x[n]
    v = berlekamp_massey(x)
    if len(v) == 0:
        return 0
    return get_nth(v, x, n)
