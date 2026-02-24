@echo off
start /high /affinity 0x1 /B /wait "prng-benchmark" "..\build\prng-benchmark.exe" ^
--benchmark_enable_random_interleaving=true ^
--benchmark_repetitions=12 ^
--benchmark_min_time=0.25s ^
--benchmark_min_warmup_time=0.5 ^
--benchmark_out=../results/results.json ^
--benchmark_out_format=json ^
--benchmark_report_aggregates_only=true
pause