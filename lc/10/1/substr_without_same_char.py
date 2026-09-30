# https://leetcode.cn/problems/longest-substring-without-repeating-characters/description/?envType=company&envId=bytedance&favoriteSlug=bytedance-thirty-days
from collections import defaultdict


class Solution:
    def lengthOfLongestSubstring(self, s: str) -> int:

        cnt = 0
        d = defaultdict(int)
        n = len(s)
        l = 0
        ans = 0
        for r in range(n):
            ch = s[r]
            d[ch] += 1
            if d[ch] == 2:
                cnt += 1
            while cnt > 0 and l <= r:
                c = s[l]
                d[c] -= 1
                l += 1
                if d[c] == 1:
                    cnt -= 1
            ans = max(ans, r - l + 1)
        return ans


s = Solution()

print(s.lengthOfLongestSubstring("aabbbcab"))
