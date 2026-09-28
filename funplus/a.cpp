#include <bits/stdc++.h>
using namespace std;


class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 判断给定的扑克牌通否通过排列和四则运算得到指定的值
     * @param cards int整型vector 扑克牌对应的数字
     * @param points int整型 扑克牌需要通过排列和四则运算得到的值
     * @return bool布尔型
     */
    bool ans = true;
    bool judgePoints(vector<int>& cards, int points) {
        // write code here
        ans = false;

        int Q = cards.size();
        Q = (1 << Q);
        vector<set<double>> dp(Q + 1);
		for (int i = 0; i < cards.size(); i++) {
			dp[(1 << i)].insert(cards[i]);
		}

        auto dfs = [&](auto&& self, int state) -> void {
            if (dp[state].size() > 0) return;
            int V = state;
            int T = V;
            set<double> tmp;
            while (V > 0) {
				if ((V ^ state) != 0) {
					self(self, V);
					self(self, V ^ state);
				}
                for (auto x: dp[V]) {
                    for (auto y: dp[V ^ state]) {
                        tmp.insert(x + y);
                        tmp.insert(x - y);
                        tmp.insert(y - x);
                        tmp.insert(x * y);
                        if (fabs(y) > 1e-6) tmp.insert(x / y);
                        if (fabs(x) > 1e-6) tmp.insert(y / x);
                    }
                }
                V = T & (V - 1);
            }
            dp[state] = std::move(tmp);
        };

        dfs(dfs, Q - 1);
        auto k = dp[Q - 1].lower_bound(points * 1.0);

        if (k != dp[Q - 1].end() && fabs(*k - points) < 1e-6) {
            return true;
        }
        return false;
    }

};


int main() {
	vector<int> d = {1, 2, 3, 8};
	int points = 24;
	Solution s;
	cout << s.judgePoints(d, points) << "\n";
}