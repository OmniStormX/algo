#!/usr/bin/env python3
"""区间括号取反 / 合法性查询：随机对拍器。

默认编译当前目录下的 main.cpp，然后与朴素算法比较。
"""

from __future__ import annotations

import argparse
import random
import subprocess
import sys
import tempfile
import time
from dataclasses import dataclass
from pathlib import Path


@dataclass
class Case:
    n: int
    initial: str
    operations: list[tuple[str, int, int]]

    def as_input(self) -> str:
        lines = [f"{self.n} {len(self.operations)}", self.initial]
        lines.extend(f"{op} {left} {right}" for op, left, right in self.operations)
        return "\n".join(lines) + "\n"


def brute_force(case: Case) -> list[str]:
    chars = list(case.initial)
    answers: list[str] = []

    for op, left, right in case.operations:
        left -= 1
        right -= 1

        if op == "F":
            for i in range(left, right + 1):
                chars[i] = ")" if chars[i] == "(" else "("
            continue

        balance = 0
        valid = True
        for i in range(left, right + 1):
            balance += 1 if chars[i] == "(" else -1
            if balance < 0:
                valid = False
                break
        answers.append("YES" if valid and balance == 0 else "NO")

    return answers


def directed_cases() -> list[tuple[str, Case]]:
    return [
        (
            "跨块查询必须检查最终余额为 0",
            Case(4, "((((", [("Q", 1, 4)]),
        ),
        (
            "rebuild 下推懒标记后必须清空 tag",
            Case(
                9,
                "((()(((((",
                [("F", 1, 9), ("Q", 4, 5), ("Q", 4, 5)],
            ),
        ),
        (
            "取反需要原串最大前缀和，不能使用最大后缀和",
            Case(
                9,
                "(()(())((",
                [("F", 3, 8), ("Q", 3, 8)],
            ),
        ),
    ]


def random_case(rng: random.Random, max_n: int, max_q: int) -> Case:
    n = rng.randint(1, max_n)
    q = rng.randint(1, max_q)
    initial = "".join(rng.choice("()") for _ in range(n))
    operations: list[tuple[str, int, int]] = []

    for index in range(q):
        # 保证每个测试至少有一次查询。
        op = "Q" if index == q - 1 else rng.choice(("F", "Q"))
        left = rng.randint(1, n)
        right = rng.randint(left, n)
        operations.append((op, left, right))

    return Case(n, initial, operations)


def compile_program(source: Path, output: Path) -> None:
    command = [
        "g++",
        "-std=c++20",
        "-O2",
        "-pipe",
        "-Wall",
        "-Wextra",
        str(source),
        "-o",
        str(output),
    ]
    result = subprocess.run(command, text=True, capture_output=True)
    if result.returncode != 0:
        print("编译失败：", file=sys.stderr)
        print(result.stderr, file=sys.stderr)
        raise SystemExit(1)


def run_program(executable: Path, test_input: str, timeout: float) -> tuple[list[str], str]:
    try:
        result = subprocess.run(
            [str(executable)],
            input=test_input,
            text=True,
            capture_output=True,
            timeout=timeout,
        )
    except subprocess.TimeoutExpired:
        return [], "程序运行超时"

    if result.returncode != 0:
        detail = f"程序异常退出，返回码 {result.returncode}"
        if result.stderr.strip():
            detail += f"\nstderr:\n{result.stderr}"
        return [], detail

    return result.stdout.split(), ""


def check_case(
    executable: Path,
    case: Case,
    timeout: float,
    label: str,
    seed: int,
) -> bool:
    test_input = case.as_input()
    expected = brute_force(case)
    actual, runtime_error = run_program(executable, test_input, timeout)

    if not runtime_error and actual == expected:
        return True

    Path("failed_case.txt").write_text(test_input, encoding="utf-8")
    print("\n发现错误！")
    print(f"测试：{label}")
    print(f"随机种子：{seed}")
    print("失败输入已写入 failed_case.txt")
    print("\n===== 输入 =====")
    print(test_input, end="")
    print("===== 正确输出 =====")
    print("\n".join(expected))
    print("===== 你的输出 =====")
    if runtime_error:
        print(runtime_error)
    else:
        print("\n".join(actual))
    return False


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="括号区间取反题随机对拍器")
    parser.add_argument("source", nargs="?", default="main.cpp", help="待测 C++ 源码")
    parser.add_argument("--tests", type=int, default=10_000, help="随机测试次数")
    parser.add_argument("--max-n", type=int, default=50, help="随机 n 的最大值")
    parser.add_argument("--max-q", type=int, default=100, help="随机 q 的最大值")
    parser.add_argument("--seed", type=int, default=None, help="随机种子，用于复现")
    parser.add_argument("--timeout", type=float, default=2.0, help="单次运行超时秒数")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    source = Path(args.source).resolve()
    if not source.is_file():
        raise SystemExit(f"找不到源码文件：{source}")
    if args.tests < 0 or args.max_n < 1 or args.max_q < 1 or args.timeout <= 0:
        raise SystemExit("参数范围不合法")

    seed = args.seed if args.seed is not None else time.time_ns()
    rng = random.Random(seed)
    print(f"seed = {seed}")

    with tempfile.TemporaryDirectory(prefix="bracket_duipai_") as temp_dir:
        executable = Path(temp_dir) / "candidate"
        compile_program(source, executable)

        for name, case in directed_cases():
            if not check_case(executable, case, args.timeout, f"定向用例：{name}", seed):
                raise SystemExit(1)
        print("定向用例全部通过")

        for test_id in range(1, args.tests + 1):
            case = random_case(rng, args.max_n, args.max_q)
            if not check_case(executable, case, args.timeout, f"随机用例 #{test_id}", seed):
                raise SystemExit(1)
            if test_id % 1000 == 0:
                print(f"已通过 {test_id} 组随机测试")

    print(f"全部通过：{args.tests} 组随机测试")


if __name__ == "__main__":
    main()
