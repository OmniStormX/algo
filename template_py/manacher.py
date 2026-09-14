def manacher(s):
    p = ["#"]
    for c in s:
        p.append(c)
        p.append("#")
    n = len(p)
    d = [1] * n
    r = 0
    l = 0
    i = 0
    while i < n:
        r = min(r - i, d[l + r - i])
        while i + d[i] < n and i - d[i] >= 0 and p[i + d[i]] == p[i - d[i]]:
            d[i] += 1
        l = i - d[i] + 1
        r = i + d[i] - 1
        i += 1
    return d
