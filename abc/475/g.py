from bisect import bisect_right

PRIMES = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71]


def normal_best(N):
    """
    不加任何限制：
    求 <= N 中约数最多的一个整数。
    """
    best_cnt = 1
    best_x = 1

    def dfs(pos, mx, cur, cnt):
        nonlocal best_cnt, best_x

        if cnt > best_cnt:
            best_cnt = cnt
            best_x = cur

        if pos == len(PRIMES):
            return

        p = PRIMES[pos]
        x = cur

        for e in range(1, mx + 1):
            if x > N // p:
                break

            x *= p
            dfs(pos + 1, e, x, cnt * (e + 1))

    dfs(0, 64, 1, 1)

    return best_cnt, best_x


def solve(N, D):

    # --------------------------------------------------
    # 先求无约束最优解
    # --------------------------------------------------

    cnt0, x0 = normal_best(N)

    if x0 % D != 0:
        return x0

    # 此时 D | x0
    # 所以 D 的质因子一定都很小，可以直接试除
    fac = []

    tmp = D

    for p in PRIMES:
        if tmp % p == 0:
            a = 0

            while tmp % p == 0:
                tmp //= p
                a += 1

            fac.append((p, a))

    # 因为 D | x0，所以一定已经完全分解
    assert tmp == 1

    ans_cnt = 0
    ans_x = 1

    # --------------------------------------------------
    # 枚举哪个 p^a 没凑够
    # --------------------------------------------------

    for ban, a in fac:

        # 可以使用的 ban 的指数：
        # 0,1,...,a-1
        pw = [1]

        for _ in range(a - 1):
            pw.append(pw[-1] * ban)

        primes = [p for p in PRIMES if p != ban]

        def dfs(pos, mx, cur, cnt):
            nonlocal ans_cnt, ans_x

            # 当前 y = cur
            #
            # 给 ban 尽可能大的指数 e < a
            # 要满足 cur * ban^e <= N

            lim = N // cur

            e = bisect_right(pw, lim) - 1

            total_cnt = cnt * (e + 1)
            x = cur * pw[e]

            if total_cnt > ans_cnt:
                ans_cnt = total_cnt
                ans_x = x

            if pos == len(primes):
                return

            p = primes[pos]
            v = cur

            for ne in range(1, mx + 1):
                if v > N // p:
                    break

                v *= p

                dfs(pos + 1, ne, v, cnt * (ne + 1))

        dfs(0, 64, 1, 1)

    return ans_x


T = int(input())

for _ in range(T):
    N, D = map(int, input().split())
    print(solve(N, D))
