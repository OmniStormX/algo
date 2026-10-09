// https://leetcode.cn/problems/minimum-sum-of-squared-difference
#include <bits/stdc++.h>
#define debug(x)    std::cerr << #x << " = " << x << std::endl
using namespace std;
class Solution {
    using i64 = long long;
public:
    long long minSumSquareDiff(vector<int>& nums1, vector<int>& nums2, int k1, int k2) {
        i64 l = -1, r = 1e18;
        i64 sum = k1 + k2;
        int n = nums1.size();
        vector<i64> nums(n);
        for (int i = 0; i < n; i++) nums[i] = abs(nums2[i] - nums1[i]);

        sort(nums.begin(), nums.end(), std::greater<>());

        i64 pre = 1e9 + 1;
        i64 cnt = 0;
        i64 ans = 0;
        for (int i = 0; i < n; i++) {
            ans += nums[i] * nums[i];
        }

        auto calc = [&](i64 pre, i64 temp, i64 cnt) {
            return cnt * (pre * pre - temp * temp);
        };

        for (int i = 0; i < n; i++) {
            if (sum >= (pre - nums[i]) * cnt) {
                sum -= (pre - nums[i]) * cnt;
                // n * pre * pre -> n * nums[i] * nums[i]
                // n * (nums[i] + (pre - nums[i]))
                ans -= calc(pre, nums[i], cnt);
                pre = nums[i];
                cnt++;
            } else {
                i64 p0 = sum / cnt, p1 = sum % cnt, p2 = cnt - p1;
                ans -= calc(pre, pre - p0 - 1, p1);
                ans -= calc(pre, pre - p0, p2);
                sum = 0;
                break;
            }
        }

        if (sum > 0) {
            i64 p0 = sum / cnt, p1 = sum % cnt, p2 = cnt - p1;
            ans -= calc(pre, max(pre - p0 - 1, 0ll), p1);
            ans -= calc(pre, max(pre - p0, 0ll), p2);
            sum = 0;
        }
        return ans;
    }
};