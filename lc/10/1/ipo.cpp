// https://leetcode.cn/problems/ipo/description


#include <algorithm>
#include <queue>
class Solution {
public:
    int findMaximizedCapital(int k, int w, std::vector<int>& profits, std::vector<int>& capital) {
        int n = profits.size();

        std::priority_queue<int, std::vector<int>, std::less<int>> q;

        std::vector<std::pair<int, int>> a(n);
        for (int i = 0; i < n; i++) {
            a[i] = {capital[i], profits[i]};
        }
        std::sort(a.begin(), a.end());
        int cur = 0;
        for (int i = 0; i < k; i++) {
            while (cur < n && a[cur].first <= w) {
                q.push(a[cur].second);
                cur++;
            }
            if (q.size()) {
                w += q.top();
                q.pop();
            }
        }

        return w;
    }
};