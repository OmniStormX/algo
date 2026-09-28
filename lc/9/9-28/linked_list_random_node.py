# Definition for singly-linked list.
# https://leetcode.cn/problems/linked-list-random-node/
import random

r"""
蓄水池算法

有一组 n 个数，要求选出 m 个数，其中每个数在 m 中的概率相同，均为 m / n

那么可以这么做：

1. 从前往后遍历 n 个数
2. 若遍历的数 i <= m，那么当前蓄水池没满，直接加入到池中
3. 当 i > m 的时候，对于蓄水池中的每个数，需要构造一个概率让它留在池中的几率为 m / n
4. 注意到原本在池子中的 a_i ，如果这个数想要留在池中，那么可以构造一个概率 (m / (m + 1)) * ((m + 1) / (m + 2)) * ..... * ((n - 1) / n) = m / n 的概率留在池中。
5. 那么构造，当 i > m 的时候，投掷一个随机数 x (x \in [1, i]) 若 x > m, 意味着这个数可以直接舍去，当 x <= m 的时候，那么这个数替代原蓄水池中 x 的位置的数。
6. 那么当 i > m 的数留在蓄水池中的概率是多少呢？可以看到，为 (m / i) * ((i) / (i + 1)) * ..... * ((n - 1) / n) = m / n 概率还是相等的。


"""


class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next


class Solution:

    def __init__(self, head: ListNode | None):
        self.root = head

    def getRandom(self) -> int:
        cnt = 1
        i = 0
        cur = self.root
        tmp = self.root
        while cur:
            if i < cnt:
                tmp = cur
            else:
                x = random.randint(0, i)
                if x == 0:
                    tmp = cur
            cur = cur.next
            i += 1
        return tmp.val


# Your Solution object will be instantiated and called as such:
# obj = Solution(head)
# param_1 = obj.getRandom()
