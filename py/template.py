# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next
class Solution:

    def getMid(self, head):
        q = head
        m = head
        if not q.next:
            return q
        while q and q.next:
            q = q.next.next
            m = m.next
        tmp = m.next
        m.next = None
        return tmp

    def mergeSort(self, head):
        if not head:
            return
        m = self.getMid(head)
        r1 = self.mergeSort(head)
        r2 = self.mergeSort(m)
        head = ListNode()
        mp = head
        while r1 and r2:
            if r1.val < r2.val:
                mp.next = r1
                mp = mp.next
                r1 = r1.next
            else:
                mp.next = r2
                mp = mp.next
                r2 = r2.next
        
        while r1:
            mp.next = r1
            mp = mp.next
            r1 = r1.next
        while r2:
            mp.next = r2
            mp = mp.next
            r2 = r2.next
        return head
             

    def sortList(self, head: ListNode) -> ListNode:
        return self.mergeSort(head)