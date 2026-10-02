#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    int candy(vector<int>& ratings) {
        int n = ratings.size();
        vector<int> pre(n + 1, 0);
        vector<int> suf(n + 1, 0);
        pre[0] = 1;
        for (int i = 1; i < n; i++) {
            if (ratings[i] > ratings[i - 1]) {
                pre[i] = pre[i - 1] + 1;
            } else if (ratings[i] <= ratings[i - 1]) {
                pre[i] = 1;
            }
        }
        suf[n - 1] = 1;
        for (int i = n - 2; i >= 0; i--) {
            if (ratings[i] > ratings[i + 1]) {
                suf[i] = suf[i + 1] + 1;
            } else {
                suf[i] = 1;
            }
        }

        long long ans = 0;
        for (int i = 0; i < n; i++) {
            ans +=  max(suf[i], pre[i]);
        }
        return ans;
    }
};