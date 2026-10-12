// https://leetcode.cn/problems/single-number-ii/
#include <bits/stdc++.h>
#define debug(x)    std::cerr << #x << " = " << x << std::endl
using namespace std;
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        vector<int> d(30, 0);
        using i64 = long long;
        i64 base = (1LL << 31);
        for (int i = 0; i < nums.size(); i++) {
            i64 x = nums[i] + base, j = 0;
            while (x > 0) {
                d[j] += x % 3;
                d[j] %= 3;
                x /= 3;
                j++;
            }
        }

        i64 x = 0;
        for (long long i = 0, j = 1; i < 30; i++, j *= 3) {
            if (d[i] > 0) {
                x += d[i] * j; 
            }
        }
        return x - base;
    }

};