// https://leetcode.cn/problems/score-of-parentheses
#include <string>
class Solution {
public:
    int scoreOfParentheses(std::string s) {
        long long ans = 0;
        int i = 0;
        int n = s.size();
        int cur = 0;
        int d = 1;
        auto dfs = [&](auto&& self) -> long long {
            if (i >= n)
                return 0;
            int tmp = cur;
            long long tmp_ans = 0;
            for (; i < n; i++) {
                if (s[i] == '(') {
                    cur++;
                    i++;
                    long long ta = self(self);
                    tmp_ans += 2 * ta;
                } else {
                    cur--;
                    if (cur < tmp) {
                        break;
                    }
                }
            }
            if (tmp_ans == 0)
                tmp_ans = 1;
            return tmp_ans;
        };
        ans = dfs(dfs);
        return ans / 2;
    }
};