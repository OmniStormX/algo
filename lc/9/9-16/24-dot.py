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


# 24 点并输出答案


# class Solution:
#     def judgePoint24(self, cards: list[int]) -> bool:
#         m = [[] for i in range(16)]
#         s = [[] for i in range(16)]
#         for i in range(4):
#             m[(1 << i)].append(cards[i])
#             s[(1 << i)].append(str(cards[i]))
#         for i in range(16):
#             if i == i & -i:
#                 continue
#             j = i & (i - 1)
#             while j > 0:
#                 for i1, x in enumerate(m[i ^ j]):
#                     for i2, y in enumerate(m[j]):
#                         s1 = "(" + s[i ^ j][i1] + ")"
#                         s2 = "(" + s[j][i2] + ")"

#                         m[i].append(x + y)
#                         s[i].append(s1 + "+" + s2)
#                         m[i].append(x - y)
#                         s[i].append(s1 + "-" + s2)
#                         m[i].append(y - x)
#                         s[i].append(s2 + "-" + s1)
#                         m[i].append(x * y)
#                         s[i].append(s1 + "*" + s2)
#                         if y != 0:
#                             m[i].append(x / y)
#                             s[i].append(s1 + "/" + s2)
#                         if x != 0:
#                             m[i].append(y / x)
#                             s[i].append(s2 + "/" + s1)
#                 j = i & (j - 1)
#         # print(m[15])
#         for i, x in enumerate(m[15]):
#             if abs(x - 24) < 0.001:
#                 print(s[15][i])
#                 return True
#         return False
