import sys
from types import GeneratorType
from math import gcd
sys.setrecursionlimit(10**7)
def bootstrap(f, stk=[]):
    def wrappedfunc(*args, **kwargs):
        if stk:
            return f(*args, **kwargs)
        else:
            to = f(*args, **kwargs)
            while True:
                if type(to) is GeneratorType:
                    stk.append(to)
                    to = next(to)
                else:
                    stk.pop()
                    if not stk:
                        break
                    to = stk[-1].send(to)
            return to
 
    return wrappedfunc


out = []
I = lambda: int(sys.stdin.readline().strip())
MI = lambda: map(int, sys.stdin.readline().strip().split())
LI = lambda: list(map(int, sys.stdin.readline().strip().split()))
IS = lambda: sys.stdin.readline().strip()
M = 998244353
Inf = 10 ** 9 + 7
def qpow(x, y):
    ans = 1
    while y > 0:
        if y & 1 == 1:
            ans = ans * x % M
        y >>= 1
        x = x * x % M
    return ans
def o(x):
    global out
    if type(x) == str:
        out.append(x)
    else:
        out.append(str(x))



def solve():
    n, k = MI()

    s = ['c', 'b', 'a']
    j = 0
    k -= 1
    d = k - (n - 1)

    if d == 0:
        s = ''.join('abc'[i % 3] for i in range(n))
    else:
        r = n - d - 2
        s = 'a' * d + 'bb' + ''.join('cab'[i % 3] for i in range(r))
    # cabcab
    # 
    print(s)
        
        
    # aabbbb 2 * (n - 2)
    # abcabc (n - 1)
    # cbacbacbaab
        
t = I()

for _ in range(t):
    solve()
print('\n'.join(out))