// https://leetcode.cn/problems/minimum-add-to-make-parentheses-valid
#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    int minAddToMakeValid(string s) {
        int cur = 0;
        int ans = 0;
        int n = s.size();
        stack<char> stk;
        for (int i = 0; i < n; i++) {
            if (s[i] == '(') {
                stk.push(s[i]);
            } else {
                if (stk.size() > 0 && stk.top() == '(') {
                    stk.pop();
                } else {
                    stk.push(s[i]);
                }
            }
        }
        return stk.size();
    }
};