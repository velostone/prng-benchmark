#!/bin/bash

sudo taskset -c 1 nice -n -20 ../build/prng-benchmark \
  --benchmark_enable_random_interleaving=true \
  --benchmark_repetitions=12 \
  --benchmark_min_time=0.25s \
  --benchmark_min_warmup_time=0.1 \
  --benchmark_out=../results/results.json \
  --benchmark_out_format=json \
  --benchmark_report_aggregates_only=true
