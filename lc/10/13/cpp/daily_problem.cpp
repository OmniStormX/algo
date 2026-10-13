#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    long long maxAlternatingSum(vector<int>& nums) {
        for (auto& x: nums) if (x < 0) x *= -1;
        sort(nums.begin(), nums.end());
        long long ans = 0;
        for (int i = 0; i < nums.size(); i++) ans += 1LL * nums[i] * nums[i] * ((i + 1) * 2 <= nums.size() ? -1 : 1);
        return ans;
    }
};