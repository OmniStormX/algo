// https://leetcode.cn/problems/remove-outermost-parentheses/description/
#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    string removeOuterParentheses(string s) {
        int cur = 0;
        string ans;
        for (auto c: s) {
            if (!((cur == 0 && c == '(') || (cur == 1 && c == ')'))) {
                ans.push_back(c);
            }
            if (c == '(') cur++;
            else cur--;
        }
        return ans;
    }
};