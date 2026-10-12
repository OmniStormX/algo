// https://leetcode.cn/problems/sum-of-squares-of-special-elements
#include <bits/stdc++.h>
#define debug(x)    std::cerr << #x << " = " << x << std::endl
using namespace std;
class Solution {
public:
    using i64 = long long;
    int maxSum(vector<int>& nums, int k) {
        vector<int> c(40, 0);
        int n = nums.size();
        for (int i = 0; i < n; i++) {
            int x = nums[i];
            for (int j = 0; j < 31; j++) {
                if (x & 1) {
                    c[j]++;
                }
                x >>= 1;
            }
        }

        const int M = 1e9 + 7;
        i64 ans = 0;
        for (int i = 0; i < k; i++) {
            i64 x = 0;
            for (int j = 0; j < 31; j++) {
                if (c[j] > 0) {
                    c[j]--;
                    x += (1 << j);
                }
            }    
            ans += x * x % M;
            ans %= M;
        }
        return ans;
    }
};