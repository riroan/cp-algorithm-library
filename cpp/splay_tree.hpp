struct Node
{
    Node *l, *r, *p;
    int cnt, value, sum, ma, mi, lazy;
    bool inv;
    Node(int value = 0)
        : cnt(1), value(value), sum(value), ma(value), mi(value), inv(false), lazy(0)
    {
        l = nullptr;
        r = nullptr;
        p = nullptr;
    }

    inline bool is_root()
    {
        return p == nullptr;
    }
};

struct SplayTree
{
    Node *root;
    SplayTree()
    {
        root = nullptr;
    }

    void update(Node *x)
    {
        x->cnt = 1;
        x->sum = x->value;
        x->ma = x->value;
        x->mi = x->value;
        if (x->l)
        {
            x->cnt += x->l->cnt;
            x->sum += x->l->sum;
            x->ma = max(x->ma, x->l->ma);
            x->mi = min(x->mi, x->l->mi);
        }
        if (x->r)
        {
            x->cnt += x->r->cnt;
            x->sum += x->r->sum;
            x->ma = max(x->ma, x->r->ma);
            x->mi = min(x->mi, x->r->mi);
        }
    }

    void push(Node *x)
    {
        if (x->inv)
        {
            swap(x->l, x->r);
            x->inv = false;
            if (x->l)
                x->l->inv ^= 1;
            if (x->r)
                x->r->inv ^= 1;
        }
        if (x->lazy)
        {
            x->value += x->lazy;
            if (x->l)
            {
                x->l->lazy += x->lazy;
                x->l->sum += x->l->cnt * x->lazy;
            }
            if (x->r)
            {
                x->r->lazy += x->lazy;
                x->r->sum += x->r->cnt * x->lazy;
            }
            x->lazy = 0;
        }
    }

    void rotate(Node *x)
    {
        // x가 루트
        if (x->is_root())
            return;

        Node *p = x->p;
        Node *r = x->r;
        Node *l = x->l;

        // x가 부모의 왼쪽 자식
        if (p->l == x)
        {
            p->l = r;
            x->r = p;
            if (r)
                r->p = p;
        }

        // x가 부모의 오른쪽 자식
        if (p->r == x)
        {
            p->r = l;
            x->l = p;
            if (l)
                l->p = p;
        }

        x->p = p->p;
        p->p = x;

        // 부모의 부모
        if (x->is_root())
            root = x;
        else
        {
            if (x->p->l == p)
                x->p->l = x;
            else
                x->p->r = x;
        }
        update(p);
        update(x);
    }

    void splay(Node *x, Node *y = nullptr)
    {
        while (x->p != y)
        {
            Node *p = x->p;
            if (p->p == y)
            {
                rotate(x);
                break;
            }
            Node *g = p->p;
            if (g->l == p && p->l == x || g->r == p && p->r == x)
                rotate(p);
            rotate(x);
        }
        if (!y)
            root = x;
    }

    void reverse(int l, int r)
    {
        Node *x = gather(++l, ++r);
        x->inv ^= 1;
    }

    Node *gather(int s, int e)
    {
        find_kth(e + 1);
        auto tmp = root;
        find_kth(s - 1);
        splay(tmp, root);
        return root->r->l;
    }

    void print()
    {
        if (root == nullptr)
            return;
        int sz = root->cnt;
        cout << "VALUES-" << sz << ": ";
        for (int i = 0; i < sz; i++)
        {
            Node *x = find_kth(i);
            cout << (unsigned char)(x->value) << " ";
        }
        cout << endl;
    }

    void insert(int v, int pos = -1)
    {
        Node *node = new Node(v);
        if (root == nullptr)
        {
            root = node;
            return;
        }
        Node *cur = nullptr;
        if (pos == -1)
            cur = find_kth(root->cnt - 1);
        else
        {
            cur = find_kth(pos);
            if (cur->l)
                cur = cur->l;
            else
            {
                cur->l = node;
                node->p = cur;
                splay(node);
                return;
            }
        }

        while (cur->r != nullptr)
            cur = cur->r;
        cur->r = node;
        node->p = cur;
        splay(node);
    }

    void dfs(auto &&func, Node *x = nullptr)
    {
        if (!x)
            x = root;
        if (x->l)
            dfs(func, x->l);
        func(x);
        if (x->r)
            dfs(func, x->r);
    }

    void erase(int k)
    {
        Node *x = find_kth(k);
        Node *l = x->l;
        Node *r = x->r;
        delete x;
        if (!l && !r)
            root = nullptr;
        else if (l && r)
        {
            r->p = nullptr;
            Node *cur = r;
            while (cur->l)
                cur = cur->l;
            l->p = cur;
            cur->l = l;
            splay(l);
        }
        else if (l)
        {
            l->p = nullptr;
            splay(l);
        }
        else
        {
            r->p = nullptr;
            splay(r);
        }
    }

    Node *find_kth(int k)
    {
        k++;
        Node *x = root;
        push(x);
        while (1)
        {
            while (x->l && x->l->cnt >= k)
            {
                x = x->l;
                push(x);
            }

            if (x->l)
                k -= x->l->cnt;

            if (!--k)
                break;
            x = x->r;
            push(x);
        }
        splay(x);
        return root;
    }

    void add(int l, int r, int v)
    {
        Node *x = gather(++l, ++r);
        x->sum += x->cnt * v;
        x->lazy += v;
    }

    void add(int ix, int v)
    {
        Node *x = find_kth(++ix);
        x->value += v;
        x->sum += v;
    }

    void set(int ix, int v)
    {
        Node *x = find_kth(++ix);
        v -= x->value;
        x->value += v;
        x->sum += v;
    }

    void shift_right(int l, int r, int k)
    {
        reverse(l, r);
        reverse(l, l + k - 1);
        reverse(l + k, r);
    }

    void shift_left(int l, int r, int k)
    {
        reverse(l, r);
        reverse(r - k + 1, r);
        reverse(l, r - k);
    }

    void erase(int l, int r)
    {
        Node *x = gather(++l, ++r)->p;
        // !! memory leak
        x->l = nullptr;
        update(x);
        splay(x);
    }
};
