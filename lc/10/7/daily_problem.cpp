// https://leetcode.cn/problems/remove-invalid-parentheses

class Solution {
public:
    vector<string> removeInvalidParentheses(string s) {
        int n = s.size();
        stack<int> l;
        int state = 0;
        for (int i = 0; i < n; i++) {
            if (s[i] == '(' or s[i] == ')') {
                state |= (1 << i);
            }
        }

        int U = state;
        string tmp;
        int mx = 0;
        auto check = [&](int x) {
            tmp = "";
            int cur = 0;
            for (int i = 0; i < n; i++) {
                if ((x & 1) == 0) {
                    tmp.push_back(s[i]);
                    if (s[i] == '(') cur++;
                    else if (s[i] == ')') cur--;
                    if (cur < 0) return false;
                }
                x >>= 1;
            }
            if (cur != 0)
                return false;
            return true; 
        };
        vector<string> ans;
        set<string> sans;
        while (state >= 0) {
            if (check(state)) {
                if (mx < tmp.size()) {
                    ans.clear();
                    sans.clear();
                    mx = tmp.size();
                    ans.push_back(tmp);
                    sans.insert(tmp);
                } else if (mx == tmp.size()) {
                    if (sans.count(tmp) == 0) {
                        ans.push_back(tmp);
                        sans.insert(tmp);
                    }
                }
            }

            if (state == 0) 
                break;
            state = (state - 1) & U;
        }
        return ans;
    }
};
