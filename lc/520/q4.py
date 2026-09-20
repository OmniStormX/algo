class Trie01:
    def __init__(self, n):
        self.t = [0] * (n + 1)
        self.l = [0] * (n + 1)
        self.r = [0] * (n + 1)
        self.cur = 0
        self.D = 14

    def newNode(self):
        self.cur += 1
        return self.cur

    def insert(self, x):
        u = 0
        d = self.D
        while d >= 0:
            if x >> d & 1 == 1:
                if self.r[u] == 0:
                    self.r[u] = self.newNode()
                u = self.r[u]
            else:
                if self.l[u] == 0:
                    self.l[u] = self.newNode()
                u = self.l[u]
            self.t[u] += 1
            d -= 1
        return u

    def ans(self):
        power = [0] * (15)
        # print(self.t)
        u = 0
        d = self.D
        while d >= 0:
            while d >= 0 and self.r[u] == 0:
                u = self.l[u]
                d -= 1
            u = self.r[u]
            power[14 - d] = self.t[u]
            d -= 1
        return power


class Solution:
    def largestPower(self, nums: list[int]) -> list[int]:
        n = len(nums)
        x = max(nums)

        l = []
        r = list(nums)
        power = [0] * 15
        pre = 0
        while x > 0:
            d = x.bit_length() - 1
            ll = []
            rr = []
            xx = x
            for y in r:
                if y >> d & 1 == 1:
                    rr.append(y)
                    xx &= y
                else:
                    ll.append(y)
            for i in range(len(rr)):
                rr[i] ^= x
            rr.sort(key=lambda x: -x)
            xxx = xx ^ x

            power[14 - d] = len(rr)


# 最大放最左侧，
# 然后枚举这个最大的若干位，

s = Solution()
print(s.largestPower([7, 5]))
print(s.largestPower([3, 1, 7]))
