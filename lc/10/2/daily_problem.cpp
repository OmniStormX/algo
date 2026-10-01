// https://leetcode.cn/problems/generate-parentheses
#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::string;
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        n *= 2;
        int u = 1 << n;
        vector<string> ans;

        for (int i = 0; i < u; i++) {
            int x = i;
            int cur = 0;
            bool f = 1;
            for (int j = 0; j < n; j++) {
                if (x & 1) {
                    cur++;
                } else {
                    cur--;
                    if (cur < 0) {
                        f = false;
                        break;
                    }
                }
                x >>= 1;
            }
            if (f && cur == 0) {
                string tmp(n, '(');
                x = i;
                for (int j = 0; j < n; j++) {
                    if ((x & 1) == 0) {
                        tmp[j] = ')';
                    }
                    x >>= 1;
                }
                ans.push_back(tmp);
            }
        }
        return ans;
    }
};

int main() {
	Solution s;
	s.generateParenthesis(1);
	return 0;
}