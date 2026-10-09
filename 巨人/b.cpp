#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;
class Solution {
  public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     *
     * @param n int整型
     * @param pre int整型vector
     * @param cost int整型vector
     * @param value int整型vector
     * @param W int整型
     * @return int整型
     */
    int maxPower(int n, vector<int>& pre, vector<int>& cost, vector<int>& value,
                 int W) {
        vector<vector<int>> e(n + 1);
        // vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
        for (int i = 1; i <= n; i++) e[pre[i]].push_back(i);
        cost[0] = value[0] = 0;
        std::function<vector<int>(int)> dfs = [&](int u) -> vector<int> {
            vector<int> dpu(W + 1, 0); 
            for (auto v : e[u]) {
                auto dpv = dfs(v);
                vector<int> tmp = dpu;
                for (int k = 1; k <= W; k++) {
                    for (int j = 1; j <= k; j++) {
                        tmp[k] = max(tmp[k], dpv[j] + dpu[k - j]);
                    }
                }
                dpu = std::move(tmp);
            }
            for (int i = W; i >= cost[u]; i--) {
                dpu[i] = dpu[i - cost[u]] + value[u];
            }
            for (int i = 0; i < cost[u] && i <= W; i++) {
                dpu[i] = 0;
            }
            return dpu;
        };
        return dfs(0)[W];
    }
};
