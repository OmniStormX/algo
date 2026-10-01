# https://leetcode.cn/problems/insert-delete-getrandom-o1
from random import randint


class RandomizedSet:

    def __init__(self):
        self.d = {}
        self.a = []

    def insert(self, val: int) -> bool:
        if val in self.d:
            return False
        self.d[val] = len(self.a)
        self.a.append(val)
        return True

    def remove(self, val: int) -> bool:
        if val not in self.d:
            return False
        p = self.d[val]
        self.d[self.a[-1]] = p
        self.d.pop(val)
        self.a[p], self.a[-1] = self.a[-1], self.a[p]
        self.a.pop()
        return True

    def getRandom(self) -> int:
        x = len(self.a) - 1
        t = randint(0, x)
        return self.a[t]


# Your RandomizedSet object will be instantiated and called as such:
# obj = RandomizedSet()
# param_1 = obj.insert(val)
# param_2 = obj.remove(val)
# param_3 = obj.getRandom()
