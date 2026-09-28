# Definition for singly-linked list.
# https://leetcode.cn/problems/reorder-list/


class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next


class Solution:

    def reverse(self, head):
        cur = head
        pre = None
        while cur:
            tmp = cur.next
            cur.next = pre
            pre = cur
            cur = tmp
        return pre

    def reorderList(self, head: ListNode | None) -> None:
        """
        Do not return anything, modify head in-place instead.
        """
        slow, quick = head, head
        while quick.next and quick.next.next:
            slow = slow.next
            quick = quick.next.next
        # print(f's.val = {slow.val}')
        s = slow.next
        s = self.reverse(s)
        # print(s.val)
        cur = head
        while s != None and cur.next != s:
            tmp = cur.next
            cur.next = s
            tmp_s = s.next
            s.next = tmp
            cur, s = tmp, tmp_s
        if cur.next == s and s:
            s.next = None
        else:
            cur.next = None
        return

        return
