# tester.sh Guide — push_swap

Based on the `main` version reviewed on September 13, 2026. [tester.sh source](https://github.com/doniu112/42_push_swap/blob/main/42%20push%20swap%20tester/tester.sh), Git blob ID: `98218815b68905633f50cc6ba84fe09d3de90bff`.

## 1. Purpose

This Bash script checks sorting with an external checker, counts operations, and runs fixed regression cases and random test series. Failures are saved to `errors.log`. Large inputs are tested using the default adaptive strategy without a selector flag.

## 2. Setup and execution

Requirements: Bash, a compiled `push_swap`, a checker, and standard utilities: `awk`, `xargs`, `mktemp`, `cat`, `wc`, and `rm`. Use Linux or WSL in VS Code. Do not run the script with `sh`: it uses Bash arrays and syntax.

If you have not cloned the repository:

```bash
git clone git@github.com:doniu112/42_push_swap.git
cd 42_push_swap
```

SSH requires a configured GitHub SSH key. Alternatively, use `https://github.com/doniu112/42_push_swap.git` as the clone URL.

For an existing checkout, open the project root containing the Makefile. Place a separately obtained checker there, then run:

```bash
make
chmod +x checker_linux
bash "42 push swap tester/tester.sh"
```

The terminal's working directory matters: the script looks for `./push_swap` and `./checker` or `./checker_linux` relative to it. Do not change into the tester subdirectory. Quote the script path because it contains spaces.

If both checkers are executable, `./checker` takes precedence. The selected path is displayed at startup. The script does not accept `--checker`, `--quick`, or `--valgrind`; configuration uses environment variables.

## 3. Test coverage

| Section | Default count | Input | Strategies |
| --- | ---: | --- | --- |
| REGRESSION | 24 | Six fixed inputs, each tested with four strategies. | simple, medium, complex, adaptive without a flag |
| EDGE | 15 | Random size 0–3; unique values from −50 to 49. | Adaptive without a flag |
| SMALL | 888 | 222 samples per strategy; size 4 or 5; values from −1000 to 999. | All four |
| LARGE | 55 | Default size 500; values from −50,000 to 49,999. | Adaptive without a flag |
| MEDIUM-LARGE | 2222 | Random size 100–500; values from −100,000 to 99,999. | Adaptive without a flag |
| Total | **3204** | With the default configuration. | |

Fixed regression inputs:

```text
-17
-35 23
-568 331 485 875
-674 -115 212 560 908
2 1
3 1 2
```

The first four are already sorted, so no operations are needed. The other two require sorting. These regressions help catch the earlier tester bug that appended an empty instruction to empty output.

Because sizes are random, EDGE does not guarantee that every size appears in a single run. Numbers are unique within each input, but inputs may repeat across samples.

## 4. Pass conditions

For valid, nonempty input:

- `push_swap` must exit with status 0 and empty stderr.
- The checker receives the exact bytes from program stdout, without an added newline.
- The checker must exit with status 0, have empty stderr, and return `OK` after Bash strips trailing newlines.

With no arguments, the checker is not run. The program must exit with status 0 and empty stdout and stderr.

An empty `Operations` section is valid for sorted input. However, this script does not independently enforce zero operations for such input: additional moves can pass if they leave the stack sorted and the checker returns OK. `test.py` enforces that requirement strictly.

## 5. Configuration without editing the script

| Variable | Default | Meaning |
| --- | ---: | --- |
| `EDGE_TESTS` | 15 | Number of samples with size 0–3. |
| `SMALL_TESTS_PER_STRATEGY` | 222 | Number of samples for each of the four strategies. |
| `LARGE_TESTS` | 55 | Number of fixed-size samples. |
| `LARGE_SIZE` | 500 | Size for LARGE; allowed range 1–100000. |
| `MEDIUM_LARGE_TESTS` | 2222 | Number of samples with size 100–500. |
| `ERROR_LOG` | `errors.log` | Failure log path; the file is cleared at startup. |

Sample counts may be 0 to skip a section. Numeric settings allow up to six digits, without a sign or leading zeros. `LARGE_SIZE=0` is invalid even when `LARGE_TESTS=0`. Very large sizes may exceed the system's argument-length limit and be expensive for the program.

Short run — 139 tests:

```bash
EDGE_TESTS=10 SMALL_TESTS_PER_STRATEGY=20 LARGE_TESTS=5 MEDIUM_LARGE_TESTS=20 \
  bash "42 push swap tester/tester.sh"
```

Only the 24 fixed regressions:

```bash
EDGE_TESTS=0 SMALL_TESTS_PER_STRATEGY=0 LARGE_TESTS=0 MEDIUM_LARGE_TESTS=0 \
  bash "42 push swap tester/tester.sh"
```

Regressions plus 100 samples of 500 numbers — 124 tests:

```bash
EDGE_TESTS=0 SMALL_TESTS_PER_STRATEGY=0 LARGE_TESTS=100 LARGE_SIZE=500 MEDIUM_LARGE_TESTS=0 \
  bash "42 push swap tester/tester.sh"
```

These assignments apply to that invocation only. Do not put spaces around `=` or after a line-continuation `\`. The total number of tests is:

```text
24 + EDGE_TESTS + 4 × SMALL_TESTS_PER_STRATEGY + LARGE_TESTS + MEDIUM_LARGE_TESTS
```

## 6. Operation counts and ratings

| Size | Excellent | Good | Pass | Above target |
| --- | --- | --- | --- | --- |
| 100 | <700 | 700–1499 | 1500–1999 | ≥2000 |
| 500 | <5500 | 5500–7999 | 8000–11999 | ≥12000 |

LARGE rates performance only for exactly 100 or 500 elements. Other sizes display `Not rated for this size`. MEDIUM-LARGE applies thresholds only for size 500; other sizes display OK without an operation-count rating.

Ratings are informational and do not increase the failure count. The final result can be SUCCESSFUL even when an operation limit is exceeded. To enforce adaptive limits, use the separate `test.py` with `--strict-performance`.

The script generates new random inputs for successive strategies and has no seed option. SMALL results are not a comparison of all four algorithms on identical random inputs. Use the random group in `test.py` for that comparison.

## 7. Saving results

The default `errors.log` contains failed tests only: section and sample number, strategy, size, input values, a reproduction command, failure reason, exit statuses, stderr from both programs, checker output, and operations. An empty log after a completed run means no failed cases were recorded.

Each new run clears the previous log, even before checking executable availability. Use separate filenames to preserve earlier results:

```bash
ERROR_LOG="errors_$(date +%Y%m%d_%H%M%S).log" \
  bash "42 push swap tester/tester.sh"
```

Read the failure log:

```bash
less errors.log
```

Capture all terminal output while preserving the tester's exit status:

```bash
set -o pipefail
bash "42 push swap tester/tester.sh" 2>&1 | tee tester_run.log
status=${PIPESTATUS[0]}
printf 'Tester exit status: %s\n' "$status"
```

The script always emits ANSI color codes, including when redirected to a file. Use `less -R tester_run.log` to view the full log. The failure log does not contain the tester's colored status labels.

## 8. Troubleshooting and limitations

| Situation | Action |
| --- | --- |
| Missing `push_swap` | Return to the directory containing the Makefile and run `make`. |
| Missing checker or permissions | Place the checker in the project root and run `chmod +x checker_linux`. |
| `Checker exited with status...` | Read checker stderr in `errors.log`. |
| `Checker did not return OK` | Inspect checker output, input values, and operations in the log; rerun the recorded command. |
| Sorted input, zero operations | This is valid; the checker should receive an empty stream. |
| The script appears stuck | There is no per-process timeout. A hanging program or checker can block the suite; interrupt with Ctrl+C. |

Exit statuses: 0 — all cases passed; 1 — failed tests or missing required executables; 2 — invalid configuration or failure to create working files; 130 — handled SIGINT interruption; 143 — SIGTERM. An interrupted run may not produce a final summary; previously recorded failures remain in the log.

This tester does not check invalid inputs, grouped arguments, bench reports, memory leaks, or Norminette. It has no internal simulator, exhaustive permutation suite, or strict-performance mode. Passing every sample does not establish correctness for every possible input.
