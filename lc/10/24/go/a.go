package main

// https://leetcode.cn/problems/reverse-nodes-in-k-group/
/**
 * Definition for singly-linked list.
 * type ListNode struct {
 *     Val int
 *     Next *ListNode
 * }
 */

type ListNode struct {
	Val  int
	Next *ListNode
}

func reverse(start, end *ListNode) {
	if start == end {
		return
	}
	var pre *ListNode = nil
	var cur *ListNode = start
	for {
		tmp := cur.Next
		cur.Next = pre
		pre = cur
		if cur == end {
			break
		}
		cur = tmp
	}
}

func reverseKGroup(head *ListNode, k int) *ListNode {
	rt := head
	var pre *ListNode = nil
	var ret *ListNode = nil
	for rt != nil {
		cur := rt
		f := true
		for i := 0; i < k-1; i++ {
			if cur.Next == nil {
				f = false
				break
			}
			cur = cur.Next
		}
		if f {
			tmp := cur.Next
			reverse(rt, cur)
			if pre != nil {
				pre.Next = cur
			}
			pre = rt
			if ret == nil {
				ret = cur
			}
			rt = tmp
		} else {
			if pre == nil {
				return rt
			} else {
				// fmt.Println("pre.val = ", pre.Val)
				pre.Next = rt
				rt = nil
			}
		}
	}
	return ret
}
