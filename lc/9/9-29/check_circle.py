# Definition for singly-linked list.
class ListNode:
    def __init__(self, x):
        self.val = x
        self.next = None


class Solution:
    def detectCycle(self, head: ListNode | None) -> ListNode | None:
        # a, b
        # a + k * b + c + d = d + q
        # a + k * b + c = 2 * (a + c + k1 * b)
        # a + c = k' * b

        quick, slow = head, head
        while quick and quick.next:
            slow = slow.next
            quick = quick.next.next
            if quick == slow:
                break
        if not quick or not quick.next:
            return None
        cur = head

        while quick != cur:
            quick = quick.next
            cur = cur.next
            if quick == cur:
                break
        return cur
