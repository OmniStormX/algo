#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include <vector>
using namespace std;

class Solution {
    using i64 = int64_t;
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 
     * @param activities long长整型vector<vector<>> 
     * @return long长整型
     */
    long long maxAward(vector<vector<long> >& activities) {
        // write code here

        vector<i64> z;
        for (auto& v : activities) {
            for (auto x: v) {
                z.push_back(x);
                z.push_back(x - 1);
            }
        }
        sort(z.begin(), z.end());
        z.erase(unique(z.begin(), z.end()), z.end());
        auto Z = [&](i64 x) {
            return lower_bound(z.begin(), z.end(), x) - z.begin() + 1;
        };

        vector<i64> dp(z.size() + 1, 0);
        unordered_map<i64, vector<vector<i64>>> m;
        for (auto v: activities) {
            i64 e = Z(v[1] - 1);
            m[e].push_back(v);
        }
        for (int i = 1; i <= z.size(); i++) {
            dp[i] = dp[i - 1];
            for (auto ac: m[i]) {
                i64 s = Z(ac[0] - 1), value = ac[2];
                dp[i] = max(dp[i], dp[s] + value);
            }
        }
        return *max_element(dp.begin(), dp.end());
    }
};