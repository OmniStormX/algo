import sys

def MI():
    return map(int, sys.stdin.readline().strip().split())

def I():
    return int(sys.stdin.readline().strip())

def solve():
    n = I()
    a = list(map(int, sys.stdin.readline().split()))
    ans = 0
    for i in range(n):
        if i > 0 and a[i] != a[i - 1]:
            ans += 1
    print(ans + 1)

for _ in range(I()):
    solve()