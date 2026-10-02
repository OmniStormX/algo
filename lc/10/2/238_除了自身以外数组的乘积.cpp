#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        vector<int> pre(n);
        pre[0] = nums[0];
        for (int i = 1; i < n; i++)
            pre[i] = pre[i - 1] * nums[i];

        int suf = 1;
        vector<int> ans(n);
        // return ans;
        for (int i = n - 1; i >= 0; i--) {
            // assert(suf != 0);
            if (i - 1 >= 0) {
                ans[i] = pre[i - 1] * suf;
            } else {
                ans[i] = suf;
            }
            // assert(nums[i])
            suf *= nums[i];
        }
        return ans;
    }
};