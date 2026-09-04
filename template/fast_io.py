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
