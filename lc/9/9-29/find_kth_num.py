# https://leetcode.cn/problems/median-of-two-sorted-arrays/description/


class Solution:
    def findMedianSortedArrays(self, nums1: list[int], nums2: list[int]) -> float:
        n = len(nums1)
        m = len(nums2)

        def f(l1, r1, l2, r2, k):
            # print(f'l1 = {l1}, r1 = {r1}, l2 = {l2}, r2 = {r2}, k = {k}')
            if r1 - l1 + 1 <= 0:
                return nums2[k - 1 + l2]
            if r2 - l2 + 1 <= 0:
                return nums1[k - 1 + l1]
            if k == 1:
                return min(nums1[l1], nums2[l2])
            m1 = min(l1 + k // 2 - 1, r1)
            m2 = min(l2 + k // 2 - 1, r2)
            if nums1[m1] <= nums2[m2]:
                if m1 - l1 + 1 < k:
                    return f(m1 + 1, r1, l2, r2, k - m1 + l1 - 1)
                else:
                    return f(l1, m1, l2, m2, k)
            else:
                if m2 - l2 + 1 < k:
                    return f(l1, r1, m2 + 1, r2, k - m2 + l2 - 1)
                else:
                    return f(l1, m1, l2, m2, k)

        n = len(nums1)
        m = len(nums2)
        if (n + m) & 1 == 1:
            return f(0, n - 1, 0, m - 1, (n + m + 1) // 2)
        else:
            # f(0, n - 1, 0, m - 1, (n + m) // 2 + 1)
            return (
                f(0, n - 1, 0, m - 1, (n + m) // 2)
                + f(0, n - 1, 0, m - 1, (n + m) // 2 + 1)
            ) / 2
