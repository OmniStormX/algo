#!/bin/bash

# pprof 压测脚本

PPROF_PORT=6060
APP_PORT=8080

echo "=== 1. 启动服务 ==="
echo "请先运行: go run b.go &"
echo ""

echo "=== 2. 压测 CPU 端点 (10 并发, 持续 30s) ==="
# 使用 ab 或 wrk 进行压测
# ab -n 10000 -c 10 http://localhost:${APP_PORT}/cpu-intensive

# 或者使用 go-wrk
# go-wrk -n 10000 -c 10 http://localhost:${APP_PORT}/cpu-intensive

# 简单循环压测
for i in {1..100}; do
    curl -s http://localhost:${APP_PORT}/cpu-intensive > /dev/null &
done
wait

echo "=== 3. 采集 CPU Profile (30秒) ==="
curl -s "http://localhost:${PPROF_PORT}/debug/pprof/profile?seconds=30" > cpu.pprof
echo "已保存到 cpu.pprof"

echo "=== 4. 采集 Heap Profile ==="
curl -s "http://localhost:${PPROF_PORT}/debug/pprof/heap" > heap.pprof
echo "已保存到 heap.pprof"

echo "=== 5. 分析 CPU (输出 top 20) ==="
go tool pprof -top cpu.pprof

echo "=== 6. 生成 CPU SVG 图 ==="
go tool pprof -svg cpu.pprof > cpu.svg
echo "已生成 cpu.svg"

echo "=== 7. 分析内存分配 ==="
go tool pprof -alloc_objects heap.pprof

echo "=== 8. 查看所有 pprof 端点 ==="
curl -s "http://localhost:${PPROF_PORT}/debug/pprof/" | grep -o 'href="[^"]*"'
