class subSet:

    def __init__(self, S):
        """U 为全集， S 为当前集合， 遍历 S 的所有在 U 条件下的子集"""
        self.U = S
        self.S = S

    def next(self):
        self.S = (self.S - 1) & self.U
        return self.S

    def get_state(self):
        return self.S

    def done(self):
        return self.S == 0


def iter_subsets(mask):
    """遍历 mask 的所有子集（包含 mask 本身和 0）"""
    sub = mask
    while True:
        yield sub
        if sub == 0:
            break
        sub = (sub - 1) & mask
