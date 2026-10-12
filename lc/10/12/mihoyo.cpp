#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    string minRemoveToMakeValid(string s) {
        auto f = [](string s) {
            int n = s.size();
            int cur = 0;
            string ans;
            for (int i = 0; i < n; i++) {
                if (s[i] == '(') {
                    cur++;
                } else if(s[i] == ')') {
                    if (cur == 0) {
                        continue;
                    } else {
                        cur--;
                    }
                }
                ans.push_back(s[i]);  
            }
            return ans;
        };
        s = f(s);
        reverse(s.begin(), s.end());
        for (auto & c: s) {
            if (c == '('  or c == ')') {
                c = '(' ^ ')' ^ c;
            }
        }
        s = f(s);
        reverse(s.begin(), s.end());
        for (auto & c: s) {
            if (c == '('  or c == ')') {
                c = '(' ^ ')' ^ c;
            }
        }
        return s;

    }
};

