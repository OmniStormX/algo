// https://leetcode.cn/problems/find-k-pairs-with-smallest-sums
#include <queue>
#include <vector>
using std::vector;
class Solution {
public:
    using i64 = long long;
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        int n = nums2.size();
        std::priority_queue<std::tuple<int, int, int>, vector<std::tuple<int, int, int>>, std::greater<std::tuple<int, int, int>>> q;

        for (int i = 0; i < n; i++) {
            q.push({nums2[i] + nums1[0], i, 0});
        }
        vector<vector<int>> ans;
        while (ans.size() < k) {
            auto [v, i, j] = q.top();
            q.pop();
            ans.push_back({nums1[j], nums2[i]});
            if (j + 1 < nums1.size()) {
                q.push({nums2[i] + nums1[j + 1], i, j + 1});
            }
        }
        return ans;
    }
};