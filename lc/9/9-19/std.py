#!/usr/bin/env python3
"""正整数数组，统计 i < j 且 a[i]*a[j] 恰有四个正因数。
python duipai.py sol.py
python duipai.py sol.py --std std.py --tests 1000
每次启动程序输入单组：n\n数组\n；输出一个整数。
"""

import argparse
import random
import subprocess
import sys
from functools import lru_cache
from math import isqrt
from pathlib import Path


@lru_cache(maxsize=100000)
def four_divisors(x):
    # 直接枚举因数，独立于优化解的分类逻辑。
    count = 0
    for d in range(1, isqrt(x) + 1):
        if x % d == 0:
            count += 1 if d * d == x else 2
            if count > 4:
                return False
    return count == 4


def brute(a):
    return sum(
        four_divisors(a[i] * a[j]) for i in range(len(a)) for j in range(i + 1, len(a))
    )


def run(path, data, timeout):
    command = (
        [sys.executable, str(path)] if path.suffix.lower() == ".py" else [str(path)]
    )
    try:
        p = subprocess.run(
            command, input=data, text=True, capture_output=True, timeout=timeout
        )
    except subprocess.TimeoutExpired:
        return False, f"TLE: 超过 {timeout} 秒"
    except OSError as e:
        return False, f"无法启动: {e}"
    if p.returncode:
        return False, f"RE: exit={p.returncode}\n{p.stderr[-4000:]}"
    tokens = p.stdout.split()
    try:
        if len(tokens) != 1:
            raise ValueError
        return True, int(tokens[0])
    except ValueError:
        return False, f"输出应为一个整数，实际输出: {p.stdout[:2000]!r}"


def cases(rng, tests, max_n, max_a):
    fixed = [
        [1],
        [2],
        [1, 1],
        [2, 2],
        [2, 3],
        [2, 4],
        [4, 2],
        [1, 8],
        [8, 1],
        [1, 6],
        [6, 1],
        [1, 4],
        [4, 1],
        [1, 9],
        [1, 27],
        [27, 1],
        [3, 9],
        [9, 3],
        [1, 16],
        [1, 12],
        [4, 9],
        [1, 2, 2, 3, 4, 6, 8],
    ]
    for a in fixed:
        if len(a) <= max_n and max(a) <= max_a:
            yield a
    pool = [
        x
        for x in [
            1,
            2,
            3,
            5,
            7,
            11,
            4,
            9,
            25,
            49,
            8,
            27,
            125,
            6,
            10,
            15,
            21,
            35,
            16,
            12,
            18,
            24,
            36,
        ]
        if x <= max_a
    ]
    for _ in range(tests):
        n = rng.randint(1, max_n)
        mode = rng.randrange(4)
        if mode == 0:
            a = [rng.randint(1, max_a) for _ in range(n)]
        elif mode == 1:
            a = [rng.choice(pool) for _ in range(n)]
        elif mode == 2:
            small = [rng.choice(pool) for _ in range(rng.randint(1, 4))]
            a = [rng.choice(small) for _ in range(n)]
        else:
            a = [1 if rng.random() < 0.5 else rng.choice(pool) for _ in range(n)]
        rng.shuffle(a)
        yield a


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "solution", type=Path, help="待测 Python 文件或已编译的可执行文件"
    )
    parser.add_argument("--std", type=Path, help="标准程序；不填则使用内置暴力解")
    parser.add_argument("--tests", type=int, default=1000, help="随机测试组数")
    parser.add_argument("--max-n", type=int, default=20)
    parser.add_argument("--max-a", type=int, default=100)
    parser.add_argument(
        "--timeout", type=float, default=3.0, help="每次程序运行的超时秒数"
    )
    parser.add_argument("--seed", type=int, default=20260919)
    args = parser.parse_args()
    if min(args.max_n, args.max_a) < 1 or args.tests < 0 or args.timeout <= 0:
        parser.error("max-n、max-a、timeout 必须为正数，tests 必须非负")
    sol = args.solution.resolve()
    std = args.std.resolve() if args.std else None
    for path in [sol] + ([std] if std else []):
        if not path.is_file():
            parser.error(f"文件不存在: {path}")
    print(f'seed={args.seed}；标准答案：{std or "内置暴力解"}', flush=True)
    total = 0
    for total, a in enumerate(
        cases(random.Random(args.seed), args.tests, args.max_n, args.max_a), 1
    ):
        data = str(len(a)) + "\n" + " ".join(map(str, a)) + "\n"
        ok_std, expected = run(std, data, args.timeout) if std else (True, brute(a))
        ok_sol, actual = run(sol, data, args.timeout)
        if not ok_std or not ok_sol or expected != actual:
            print(f"\n第 {total} 组失败（seed={args.seed}）")
            print("输入：\n" + data, end="")
            print(f"标准答案/状态：{expected}\n你的输出/状态：{actual}")
            Path("fail.in").write_text(data, encoding="utf-8")
            Path("fail.expected.txt").write_text(str(expected) + "\n", encoding="utf-8")
            Path("fail.actual.txt").write_text(str(actual) + "\n", encoding="utf-8")
            print("已保存 fail.in、fail.expected.txt、fail.actual.txt")
            return 1
        if total % 100 == 0:
            print(f"已通过 {total} 组", flush=True)
    print(f"全部通过，共 {total} 组（含固定用例）")
    return 0


if __name__ == "__main__":
    sys.exit(main())
