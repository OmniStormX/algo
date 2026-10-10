// https://leetcode.cn/problems/sum-of-squares-of-special-elements
#include <bits/stdc++.h>
#define debug(x)    std::cerr << #x << " = " << x << std::endl
using namespace std;
class Solution {
public:
    int sumOfSquares(vector<int>& nums) {
        int ans = 0;
        for (int i = 1; i * i <= nums.size(); i++) if (nums.size() % i == 0) {
            ans += nums[i - 1] * nums[i - 1];
            if (i * i != nums.size()) ans += nums[nums.size() / i - 1] * nums[nums.size() / i - 1];
        } 
        return ans;
    }
};