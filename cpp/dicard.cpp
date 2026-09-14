#include <vector>
using std::vector;
struct CartesianTree {
    int n;
    vector<int> a;
    vector<int> ls, rs, fa;
    int root;

    CartesianTree(const vector<int>& _a) {
        a = _a;
        n = (int)a.size() - 1;

        ls.assign(n + 1, 0);
        rs.assign(n + 1, 0);
        fa.assign(n + 1, 0);

        build();
    }

    void build() {
        vector<int> stk;

        for (int i = 1; i <= n; i++) {
            int last = 0;

            // 小根笛卡尔树
            while (!stk.empty() && a[stk.back()] > a[i]) {
                last = stk.back();
                stk.pop_back();
            }

            // 当前栈顶 < a[i]
            // 所以 i 成为栈顶的右儿子
            if (!stk.empty()) {
                rs[stk.back()] = i;
                fa[i] = stk.back();
            }

            // 最后弹出的节点成为 i 的左儿子
            if (last) {
                ls[i] = last;
                fa[last] = i;
            }

            stk.push_back(i);
        }

        root = stk.front();
    }
};