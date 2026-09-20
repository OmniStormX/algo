# 长度为 n 的数组 a, 1 <= a[i] <= 10^6。
# 求 i < j 且 a[i] * a[j] 的因子数为 4 的 (i, j) 的对数
import sys
from math import isqrt


def solve():
    input = sys.stdin.buffer.readline
    n = int(input())
    a = list(map(int, input().split()))
    M = max(a)

    # spf[x]：x 的最小质因子；对于质数 p，spf[p] == p
    spf = list(range(M + 1))

    for p in range(2, isqrt(M) + 1):
        if spf[p] != p:
            continue
        for x in range(p * p, M + 1, p):
            if spf[x] == x:
                spf[x] = p

    cnt = [0] * (M + 1)
    prime_count = 0  # 前面出现的质数元素数量
    four_count = 0  # 前面出现的恰有 4 个因数的元素数量
    ans = 0

    for x in a:
        if x == 1:
            # 1 与前面的 p³、pq 配对
            ans += four_count
        else:
            p = spf[x]
            y = x // p

            if y == 1:
                # x 是质数 p：与不同质数、p² 配对
                ans += prime_count - cnt[p]
                if p * p <= M:
                    ans += cnt[p * p]
                prime_count += 1

            elif spf[y] == y:
                # x = p * y，且 p、y 都是质数
                if p == y:
                    # x = p²：只能与 p 配对
                    ans += cnt[p]
                else:
                    # x = pq，p != q：只能与 1 配对
                    ans += cnt[1]
                    four_count += 1

            elif x == p * p * p:
                # x = p³：只能与 1 配对
                ans += cnt[1]
                four_count += 1

        cnt[x] += 1

    print(ans)


solve()
