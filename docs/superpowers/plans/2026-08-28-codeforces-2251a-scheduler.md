# Codeforces 2251A Scheduler Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 构建一个可直接提交的 C++17 单文件交互式调度器，并通过单元测试与题面样例的真实逐帧交互回放。

**Architecture:** `cf2251a.cpp` 内包含耗时表插值、请求状态机、事件解析和调度策略，通过 `Scheduler::Run` 驱动整个交互循环。测试以 `CF2251A_UNIT_TEST` 宏包含同一个源文件，并用 Python 子进程测试真正的逐帧输出与 flush，不引入提交时依赖。

**Tech Stack:** C++17 标准库、Python 3 标准库、GCC 警告检查。

**Spec:** `docs/superpowers/specs/2026-08-28-codeforces-2251a-scheduler-design.md`

## Global Constraints

- 最终提交文件固定为 `tmp/tmp-8-28/cf2251a.cpp`，不依赖测试文件。
- 代码使用适量中文注释，命名和结构遵循 Google C++ 风格。
- 每帧必须先完整读取，再更新状态并输出至多 `K+1` 个任务，随后显式刷新。
- 所有调度必须只依赖已经收到的事件，不能预测 `FIN` 或未来到达。
- 保留现有未提交改动，不修改本任务范围之外的文件。

---

### Task 1: 耗时表与批大小估计

**Files:**
- Create: `tests/cf2251a_unit_test.cpp`
- Create: `tmp/tmp-8-28/cf2251a.cpp`

**Interfaces:**
- Produces: `cf2251a::TimeTable::Build(const std::vector<TimeRow>&)`、`cf2251a::TimeTable::Get(Stage, int)`、`cf2251a::ChooseTargetBatch(...)`。

- [ ] **Step 1: 写入失败的耗时表测试**

```cpp
#define CF2251A_UNIT_TEST
#include "../tmp/tmp-8-28/cf2251a.cpp"

int main() {
  using namespace cf2251a;
  TimeTable table;
  table.Build({TimeRow{1, {3, 10, 2, 1, 4, 1}},
               TimeRow{4, {7, 16, 5, 4, 10, 4}}});
  assert(std::abs(table.Get(Stage::kDecodeProc, 1) - 4.0) < 1e-9);
  assert(std::abs(table.Get(Stage::kDecodeProc, 2) - 6.0) < 1e-9);
  assert(std::abs(table.Get(Stage::kDecodeProc, 8) - 10.0) < 1e-9);
}
```

- [ ] **Step 2: 运行测试并确认因生产接口不存在而失败**

Run: `g++ -std=c++17 -O2 -Wall -Wextra -Wshadow tests/cf2251a_unit_test.cpp -o /tmp/cf2251a_unit_test`

Expected: FAIL，报错指向缺少 `cf2251a.cpp` 或未定义 `TimeTable`。

- [ ] **Step 3: 实现最小耗时表与批大小估计**

在 `cf2251a.cpp` 中实现六列独立排序、忽略 `-1`、区间内线性插值、区间外使用端点值。枚举批大小 `1..2000`，以边缘计算、云端计算和双向链路瓶颈的最大值选择目标批大小。

- [ ] **Step 4: 运行测试并确认通过**

Run: `g++ -std=c++17 -O2 -Wall -Wextra -Wshadow tests/cf2251a_unit_test.cpp -o /tmp/cf2251a_unit_test && /tmp/cf2251a_unit_test`

Expected: PASS，退出码为 `0` 且无编译警告。

### Task 2: 请求状态机与合法基线调度

**Files:**
- Modify: `tests/cf2251a_unit_test.cpp`
- Modify: `tmp/tmp-8-28/cf2251a.cpp`

**Interfaces:**
- Consumes: `TimeTable::Get` 和目标批大小。
- Produces: `RequestState`、`Request`、`Assignment`、`Scheduler::Run(std::istream&, std::ostream&)`。

- [ ] **Step 1: 增加失败的 Example 1 完整回放测试**

测试向 `Scheduler::Run` 输入题面 Example 1 的完整记录，逐字比较以下输出：

```text
1
E P PRE 0 0
0
1
C0 P PROC 0 4 0 0
0
1
E P POST 0 0
1
E D PRE -1 1 0
0
1
C0 D PROC 0 1 0
0
1
E D POST -1 1 0
0
```

- [ ] **Step 2: 运行测试并确认因 `Scheduler` 缺失而失败**

Run: `g++ -std=c++17 -O2 -Wall -Wextra -Wshadow tests/cf2251a_unit_test.cpp -o /tmp/cf2251a_unit_test && /tmp/cf2251a_unit_test`

Expected: FAIL，报错或断言明确指向缺少交互状态机输出。

- [ ] **Step 3: 实现事件解析和单请求合法调度**

实现完整帧读取、`ARR/TDN/XDN/FIN` 状态变更、服务器忙闲标记以及六种任务命令。先使用完整 Prefill 和单请求 Decode，确保同帧 `TDN+FIN` 不会重新调度已完成请求。

- [ ] **Step 4: 运行测试并确认 Example 1 通过**

Run: `g++ -std=c++17 -O2 -Wall -Wextra -Wshadow tests/cf2251a_unit_test.cpp -o /tmp/cf2251a_unit_test && /tmp/cf2251a_unit_test`

Expected: PASS，完整输出与题面记录一致。

### Task 3: 负载均衡与自适应批处理

**Files:**
- Modify: `tests/cf2251a_unit_test.cpp`
- Modify: `tmp/tmp-8-28/cf2251a.cpp`

**Interfaces:**
- Consumes: 请求状态、任务表和目标批大小。
- Produces: `Scheduler::ChooseRemote`、`Scheduler::BuildEdgeAssignment`、`Scheduler::BuildCloudAssignment`。

- [ ] **Step 1: 增加失败的合批与服务器约束测试**

构造两个云服务器上的四个请求，断言：

```cpp
assert(edge_assignment.command == "E D PRE -1 4 0 1 2 3");
assert(cloud_zero.command == "C0 D PROC 0 2 0 2");
assert(cloud_one.command == "C1 D PROC 1 2 1 3");
```

另构造无在途事件且只有一个就绪请求的状态，断言调度器立即生成单请求任务而不是等待目标批大小。

- [ ] **Step 2: 运行测试并确认批处理断言失败**

Run: `g++ -std=c++17 -O2 -Wall -Wextra -Wshadow tests/cf2251a_unit_test.cpp -o /tmp/cf2251a_unit_test && /tmp/cf2251a_unit_test`

Expected: FAIL，现有基线只能生成大小为 `1` 的批次。

- [ ] **Step 3: 实现高分启发式**

实现以下行为：按估计云端工作量选择 `P PRE` 远端；`D PRE/D POST` 跨远端合批；`D PROC` 按远端分别合批；根据目标批大小、SLO 截止时间和是否存在可靠未来事件决定立即执行或等待。用等待年龄打破并列，保证所有阶段最终得到服务。

- [ ] **Step 4: 运行测试并确认合批、防 stuck 和远端约束通过**

Run: `g++ -std=c++17 -O2 -Wall -Wextra -Wshadow tests/cf2251a_unit_test.cpp -o /tmp/cf2251a_unit_test && /tmp/cf2251a_unit_test`

Expected: PASS，所有合批成员合法且无跨远端 `D PROC`。

### Task 4: 真正逐帧交互与最终验证

**Files:**
- Create: `tests/cf2251a_interactive_replay.py`
- Modify: `tmp/tmp-8-28/cf2251a.cpp`

**Interfaces:**
- Consumes: 最终调度器可执行文件。
- Produces: 逐帧发送输入、限时读取响应并校验命令的本地交互器。

- [ ] **Step 1: 编写失败的逐帧回放测试**

Python 测试使用 `subprocess.Popen` 启动程序，先发送配置和任务表，再逐帧发送 Example 1；每帧读取任务数量和对应命令后才发送下一帧。错误命令、未 flush 或超时均使测试失败。

- [ ] **Step 2: 运行回放并确认测试能够捕获缺少 flush 或格式问题**

Run: `python3 tests/cf2251a_interactive_replay.py /tmp/cf2251a`

Expected: 在正式 `main` 尚未完成时 FAIL，报告进程提前结束、超时或响应不匹配。

- [ ] **Step 3: 完成正式入口和错误退出路径**

实现普通编译下的 `main()`，启用快速 I/O；每次响应后调用 `flush`；输入 EOF 或 `END` 时返回 `0`。修复回放暴露的协议问题，不向标准输出写调试信息。

- [ ] **Step 4: 执行完整验证**

Run:

```bash
g++ -std=c++17 -O2 -pipe -Wall -Wextra -Wshadow -Wconversion \
  tmp/tmp-8-28/cf2251a.cpp -o /tmp/cf2251a
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow \
  tests/cf2251a_unit_test.cpp -o /tmp/cf2251a_unit_test
/tmp/cf2251a_unit_test
python3 tests/cf2251a_interactive_replay.py /tmp/cf2251a
```

Expected: 两次编译退出码均为 `0` 且无警告；单元测试和交互回放全部通过。
