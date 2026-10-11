// https://leetcode.cn/problems/prime-subset-selection-ii
// 需要一点理解，字典序如何 dp
#include <algorithm>
#include <bits/stdc++.h>
#include <bitset>
#define debug(x)    std::cerr << #x << " = " << x << std::endl
using namespace std;
class Solution {
public:
    vector<int> maxPrimeSubset(int n, int s) {
        vector<int> prime;
        vector<bool> isPrime(n + 1, true);
        
        for (int i = 2; i <= n; i++) {
            if (isPrime[i]) {
                prime.push_back(i);
            }

            for (auto x: prime) {
                if (i * x <= n) isPrime[i * x] = false;
                else {
                    break;
                }
            }
        }
        
        int m = prime.size();
        int Inf = 0x7ffffff;
        vector<vector<int>> dp(m + 1, vector<int>(s + 1, Inf));
        if (prime.size() == 0)
            return {};

        dp[m][0] = 0;
        
        for (int i = m - 1; i >= 0; i--) {
            for (int j = 0; j <= s; j++) {
                dp[i][j] = min(dp[i + 1][j], dp[i][j]);
                if (j - prime[i] < 0) continue;
                dp[i][j] = min(dp[i + 1][j - prime[i]] + 1, dp[i][j]);
            }
        }
        int cur = s, p = 0;
        while (cur > 0 && dp[p][cur] == Inf) cur--;
        vector<int> ans;
        while (cur > 0) {
            while (p + 1 <= m && !(cur >= prime[p] && dp[p][cur] == dp[p + 1][cur - prime[p]] + 1)) {
                p++;
            }
            ans.push_back(prime[p]);
            cur -= prime[p];
            p++;
        }
        return ans;
    }
};




int main() {
    Solution s;
    
    for (auto x: s.maxPrimeSubset(15, 18)) {
        std::cout << x << " ";
    }
}