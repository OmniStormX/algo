# https://leetcode.cn/problems/circle-and-rectangle-overlapping/
class Solution:
    def checkOverlap(
        self,
        radius: int,
        xCenter: int,
        yCenter: int,
        x1: int,
        y1: int,
        x2: int,
        y2: int,
    ) -> bool:
        xx = min(max(xCenter, x1), x2)
        yy = min(max(yCenter, y1), y2)
        if xCenter >= x1 and xCenter <= x2 and yCenter >= y1 and yCenter <= y2:
            return True
        return (xx - xCenter) ** 2 + (yy - yCenter) ** 2 <= radius**2
