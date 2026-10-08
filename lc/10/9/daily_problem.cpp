// https://leetcode.cn/problems/minimum-insertions-to-balance-a-parentheses-string
#include <bits/stdc++.h>
#define debug(x)    std::cerr << #x << " = " << x << std::endl
using namespace std;
class Solution {
public:
    int minInsertions(string s) {
        int ans = 0;
        int cur = 0;
        int n = s.size();
        for (int i = 0; i < n; i++) {
            if (s[i] == '(') {
                if (cur & 1) {
                    ans++;
                    cur--;
                }
                cur += 2;
            } else {
                if (cur == 0) {
                    ans++;
                    cur++;
                } else {
                    cur--;
                }
            }
        }
        if (cur > 0) ans += cur;
        return ans;
    }
};