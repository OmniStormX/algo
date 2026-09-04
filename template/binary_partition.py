def binaryPartition(n, g):

    color = [0] * n

    def dfs(u, c):
        color[u] = c

        for v, w in g[u]:

            if color[v] == 0:
                if not dfs(v, -c):
                    return False

            elif color[v] == color[u]:
                return False

        return True

    ans = 0
    """
        二分图最大匹配数量
        # 其中正的是在图 A, 负的在图 B
    """
    ans = 0
    for i in range(n):
        ans += dfs(i, i + 1)
