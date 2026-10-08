// https://leetcode.cn/problems/reverse-nodes-in-k-group
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    using lptr = ListNode*;

    lptr reverese(lptr start, lptr end) {
        lptr pre = nullptr, cur = start, stop = end->next;
        while (cur != stop) {
            lptr nxt = cur->next;
            cur->next = pre;
            pre = cur;
            cur = nxt;
        }
        return pre;
    }

    ListNode* reverseKGroup(ListNode* head, int k) {
        if (k == 1) return head;
        lptr r = head;
        lptr ans = head;
        lptr pre = nullptr;
        while (r != nullptr) {
            bool f = true;
            lptr tmp = r;
            for (int i = 0; i < k - 1; i++) {
                tmp = tmp->next;
                if (tmp == nullptr) {
                    f = false;
                    break;
                }
            }
            if (f) {
                lptr nxt = tmp->next;
                auto ed = reverese(r, tmp);
                if (pre) 
                    pre->next = ed;
                pre = r;
                r = nxt;
                if (ans == head) 
                    ans = ed;
            } else {
                if (pre) 
                    pre->next = r;
                break;
            }
        }
        return ans;
    }
};