def colorBipartite(n, m, g):
    ans = 0
    vis = [-1] * m
    match = [-1] * m
    tag = 0

    def dfs(u):
        for v in g[u]:
            if vis[v] == tag:
                continue
            vis[v] = tag
            if match[v] == -1 or dfs(match[v]):
                match[v] = u
                return True
        return False

    for i in range(n):
        tag += 1
        ans += dfs(i)
    return ans
