# https://leetcode.cn/problems/24-game/
class Solution:
    def judgePoint24(self, cards: list[int]) -> bool:
        m = [[] for i in range(16)]
        for i in range(4):
            m[(1 << i)].append(cards[i])

        for i in range(16):
            if i == i & -i:
                continue
            j = i & (i - 1)
            while j > 0:
                for x in m[i ^ j]:
                    for y in m[j]:
                        m[i].append(x + y)
                        m[i].append(x - y)
                        m[i].append(y - x)
                        m[i].append(x * y)
                        if y != 0:
                            m[i].append(x / y)
                        if x != 0:
                            m[i].append(y / x)
                j = i & (j - 1)
        # print(m[15])
        for x in m[15]:
            if abs(x - 24) < 0.001:
                return True
        return False
