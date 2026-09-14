#include <iostream>
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
            while (!stk.empty() && a[stk.back()] < a[i]) {
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

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	int n;
	std::cin >> n;
	vector<int> a(n);
	for (int i = 0; i < n; i++) {
		std::cin >> a[i];
	}

	auto cart = CartesianTree(a);

	vector<int> L(n), R(n), size(n);
	int ans = 0;

	auto dfs = [&](auto &&self, int root) -> void {
		if (cart.ls[root]) self(self, cart.ls[root]);
		if (cart.rs[root]) self(self, cart.rs[root]);
		L[root] = R[root] = root;
		size[root] = 1;
		if (cart.ls[root]) {
			L[root] = L[cart.ls[root]];
			size[root] += size[cart.ls[root]];
		}
		if (cart.rs[root]) {
			R[root] = R[cart.rs[root]];
			size[root] += size[cart.rs[root]];
		}
		int r1 = cart.ls[root], r2 = cart.rs[root];
		if (size[r1] > size[r2]) {
			std::swap(r1, r2);
		}

		if (r1) {
		}
	};

	dfs(dfs, cart.root);


}