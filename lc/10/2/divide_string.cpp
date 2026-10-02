// @lc code=start
#include <bits/stdc++.h>


#define debug(x)	cerr << "[debug] Line:" << __LINE__ << " " << #x << " : " << x << std::endl
using namespace std;
class Solution {
public:
    vector<string> fullJustify(vector<string>& words, int maxWidth) {
        int m = maxWidth;
        int n = words.size();

        int i = 0;
        vector<string> ans;
        while (i < n) {
            int j = i, cur = words[i].size();
			int white = 0;
            while (j + 1 < n && cur + words[j + 1].size() + 1 <= m) {
                j++;
                cur += words[j].size() + 1;
				white += 1;
            }
            int k = j - i;
			if (j != n - 1) {
				if (k == 0)  {
					string tmp = words[i];
					tmp += string(m - words[i].size(), ' ');
					ans.push_back(std::move(tmp));
				} else {
					white += m - cur;
					int q = white % k;
					int qq = white / k;
					string tmp = "";
					tmp += words[i];
					for (int l = 0; l < q; l++) {
						tmp += string(qq + 1, ' ');
						tmp += words[i + l + 1];
					}
					// debug(q);
					for (int l = q; l < k; l++) {
						tmp += string(qq, ' ');
						tmp += words[i + l + 1];
					}
					ans.push_back(std::move(tmp));
				}
				i = j + 1;
			} else {
				string tmp = words[i];
				for (int q = i + 1; q <= j; q++) {
					tmp += " ";
					tmp += words[q];
				}
				tmp += string(m - tmp.size(), ' ');
				ans.push_back(std::move(tmp));
				i = j + 1;
			}
        }
        return ans;
    }
};