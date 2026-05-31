class Solution {
public:

    i64 dp[20][4][200][10];
    i64 dfs(vector<int>&a, int t, int state, int pre, int sum, bool isNum, bool isLimit) {
        // state: 0 candidate, 1 up, 2 down, 3 none 
        if (!isLimit && isNum && t >= 0 && dp[t][state][sum][pre] != -1) return dp[t][state][sum][pre];
        if (t == -1) {
            if (!isNum) return 0;
            if ( state == 1 || state == 2 || S.find(sum) != S.end()) {
                return 1;
            } else {
                return 0;
            }
        }
        i64 ans = 0; 
        if (!isNum) {
            ans += dfs(a, t - 1, state, pre, sum, isNum, 0);
        }
        for (int i = !isNum; i <= (isLimit ? a[t] : 9); i++) {
            if (isNum) {
                if (i > pre && (state == 0 || state == 1)) {
                    ans += dfs(a, t - 1, 1, i, sum + i, isNum, i == a[t] && isLimit);       
                } else if (i < pre && (state == 0 || state == 2)) {
                    ans += dfs(a, t - 1, 2, i, sum + i, isNum, i == a[t] && isLimit);
                } else {
                    ans += dfs(a, t - 1, 3, i, sum + i, isNum, i == a[t] && isLimit);
                }
            } else {
                ans += dfs(a, t - 1, 0, i, i, true, i == a[t] && isLimit);
            }
        }
        if (!isLimit && isNum) {
            dp[t][state][sum][pre] = ans;
        }
        return ans;
    }
};