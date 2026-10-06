// https://leetcode.cn/problems/partition-list
#include <bits/stdc++.h>
using namespace std;
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
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution {
public:
    ListNode* partition(ListNode* head, int x) {
        using t = ListNode*;

        t rt1 = new ListNode(-1); // 小于 x
        t rt2 = new ListNode(-1); // 大于 x
        t pre1 = rt1;
        t pre2 = rt2;

        for (t i = head; i != nullptr; i = i->next) {
            if (i->val < x) {
                pre1->next = i;
                pre1 = i;
            } else {
                pre2->next = i;
                pre2 = i;
            }
        }
        pre1->next = nullptr;
        pre2->next = nullptr;
        t hd = rt1;
        // if (hd == nullptr) {
        //     hd = rt2;
        // }

        t tmp = hd;
        while (tmp->next != nullptr) {
            tmp = tmp->next;
        }
        t tmp2 = rt2->next;
        tmp->next = rt2->next;
        // while (tmp2 != nullptr) {
        //     tmp2 = tmp2->next;
        // }
        t ans = rt1->next;
        delete rt1;
        delete rt2;
        return ans;
    }
};