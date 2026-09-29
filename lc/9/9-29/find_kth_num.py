# https://leetcode.cn/problems/median-of-two-sorted-arrays/description/


class Solution:
    def findMedianSortedArrays(self, nums1: list[int], nums2: list[int]) -> float:
        n = len(nums1)
        m = len(nums2)

        def f(l1, r1, l2, r2, k):
            if r1 - l1 + 1 <= 0:
                return nums2[k - 1 + l2]
            if r2 - l2 + 1 <= 0:
                return nums1[k - 1 + l1]
            if k == 1:
                return min(nums1[l1], nums2[l2])
            # 如果 m1 - l1 + 1 > k / 2, 那么可能 nums[m1] 是 k 小数但是被跳过了
            # 如果 m1 - l1 + 1 <= k / 2, m2 - l2 + 1 <= k / 2, 那么如果 num[m1] <= nums2[m2] 那么 [l1, m1] 均不可能为 k 小数。
            m1 = min(l1 + k // 2 - 1, r1)
            m2 = min(l2 + k // 2 - 1, r2)
            if nums1[m1] <= nums2[m2]:
                return f(m1 + 1, r1, l2, r2, k - m1 + l1 - 1)
            else:
                return f(l1, r1, m2 + 1, r2, k - m2 + l2 - 1)

        n = len(nums1)
        m = len(nums2)
        if (n + m) & 1 == 1:
            return f(0, n - 1, 0, m - 1, (n + m + 1) // 2)
        else:
            return (
                f(0, n - 1, 0, m - 1, (n + m) // 2)
                + f(0, n - 1, 0, m - 1, (n + m) // 2 + 1)
            ) / 2
