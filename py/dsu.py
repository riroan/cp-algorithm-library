class DisjointSet:
    def __init__(self, n):
        self.f = [i for i in range(n)]
        self.siz = [1]*n

    def get(self, x):
        while x!=self.f[x]:
            self.f[x] = self.f[self.f[x]]
            x = self.f[x]
        return x

    def unite(self, x, y):
        x = self.get(x)
        y = self.get(y)
        if x == y:
            return False
        self.siz[x] += self.siz[y]
        self.f[y] = x
        return True

    def size(self, x):
        return self.siz[self.get(x)]
