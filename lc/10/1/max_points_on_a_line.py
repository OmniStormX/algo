import math
from collections import defaultdict


class Solution:
    def maxPoints(self, points: list[list[int]]) -> int:
        d = defaultdict(set)
        n = len(points)

        ans = 1
        for i in range(1, n):
            for j in range(i):
                dx, dy = points[i][0] - points[j][0], points[i][1] - points[j][1]
                x1, y1 = points[i][0], points[i][1]

                t = math.gcd(abs(-dy), abs(dx))
                a = -dy
                b = dx
                if t != 0:
                    a //= t
                    b //= t
                d[(a, b, -x1 * a - y1 * b)].add(i)
                d[(a, b, -x1 * a - y1 * b)].add(j)
                ans = max(ans, len(d[(a, b, -x1 * a - y1 * b)]))
        return ans
