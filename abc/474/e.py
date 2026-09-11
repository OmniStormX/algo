import atexit
import sys

out = []


def I():
    return int(sys.stdin.readline().strip())


def MI():
    return map(int, sys.stdin.readline().strip().split())


def LI():
    return list(map(int, sys.stdin.readline().strip().split()))


def LFI():
    return list(map(float, sys.stdin.readline().strip().split()))


def IS():
    return sys.stdin.readline().strip()


def _print(*args, sep=" ", end="\n"):
    out.append(sep.join(map(str, args)) + end)


print = _print


@atexit.register
def _():
    sys.stdout.write("".join(out))


for _ in range(I()):
    N = I()
    ab = [[0, 0] for _ in range(N)]
    ans = 0
    a_min = 10**10
    for i in range(N):
        ab[i][0], ab[i][1] = MI()
        a_min = min(a_min, ab[i][0])
    ab.sort(key=lambda x: -x[0] + x[1])
    for i in range(N):
        ans += ab[i][0]

    k = N // 2
    for i in range(k):
        ans += ab[i][1] - ab[i][0]

    ret = ans
    for i in range(k, N):
        kk = 2
        if i == k and N & 1 == 1:
            kk = 1
        if ab[i][1] + a_min * kk - ab[i][0] < 0:
            ans += ab[i][1] + a_min * kk - ab[i][0]

    print(ans)
# [
#     [[0, 0], [2, 0], [7, 1]],
#     [[0, 0], [0, 1], [0, 0]],
#     [[0, 0], [0, 0], [0, 1]],
#     [[0, 0], [0, 1], [0, 0]],
# ]
