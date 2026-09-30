# https://leetcode.cn/problems/valid-parentheses


class Solution:
    def isValid(self, s: str) -> bool:
        stk = []
        f = {"(": 0, "{": 1, "[": 2}
        ff = {")": 0, "}": 1, "]": 2}
        for c in s:
            if c in f:
                stk.append(f[c])
            else:
                if stk and stk[-1] == ff[c]:
                    stk.pop()
                else:
                    return False
        return not stk
