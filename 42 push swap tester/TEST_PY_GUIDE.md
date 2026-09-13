# test.py Guide — push_swap

Based on the `main` version reviewed on September 13, 2026. [test.py source](https://github.com/doniu112/42_push_swap/blob/main/42%20push%20swap%20tester/test.py), Git blob ID: `1da5bbba83a2cc828bd7853bbab9489ff6f3ba27`.

## 1. Purpose

This tester checks sorting using an independent Python stack simulator, validates input and flags, and checks the exact `--bench` report. It can also run an external checker, Valgrind, and Norminette. No third-party Python libraries are required.

## 2. Setup and first run

Basic requirements: Python 3.10 or newer and a compiled `push_swap`. For the complete toolset, use Linux or a WSL terminal in VS Code.

If you have not cloned the repository:

```bash
git clone git@github.com:doniu112/42_push_swap.git
cd 42_push_swap
```

SSH cloning requires a configured GitHub SSH key. Alternatively:

```bash
git clone https://github.com/doniu112/42_push_swap.git
cd 42_push_swap
```

For an existing checkout, open a terminal in the directory containing the Makefile. Run all commands below from that project root, rather than from the tester subdirectory.

```bash
make
python3 --version
python3 "42 push swap tester/test.py" --quick
```

The quotation marks are necessary because the directory name contains spaces. The tester uses `./push_swap` by default and does not compile it automatically.

## 3. Ready-to-use commands

Full tests with the Python simulator:

```bash
python3 "42 push swap tester/test.py"
```

With an external checker obtained separately and placed in the project root:

```bash
chmod +x checker_linux
python3 "42 push swap tester/test.py" --checker ./checker_linux
```

Omitting `--checker` does not disable sorting validation: the Python simulator still runs. The external checker receives separate numeric arguments, even when the program is tested with grouped arguments.

Memory and style checks, with Valgrind and Norminette installed and available in the terminal's PATH:

```bash
make re CFLAGS="-Wall -Wextra -Werror -g -O0"
python3 "42 push swap tester/test.py" --checker ./checker_linux --valgrind --norminette --source-dir .
```

Full checks with the adaptive performance requirement enabled:

```bash
python3 "42 push swap tester/test.py" --checker ./checker_linux --valgrind --norminette --strict-performance
```

Reproducible random inputs and a separate report:

```bash
python3 "42 push swap tester/test.py" --seed 123 --samples 30 --report report_seed123.json
```

After debugging, restore the normal build with `make re`.

## 4. Test coverage

| Group | Cases |
| --- | --- |
| Input and flags | No arguments; empty groups; standalone `+`/`-`; text, decimals, and hexadecimal notation; values outside INT_MIN/INT_MAX; very long numbers; duplicates including `+0`/`-0` and `01`/`1`; unknown flags; flags after numbers; two selectors, including repeated identical selectors; valid bench/strategy ordering. |
| Grouped numbers | A single quoted list, groups mixed with separate numbers, spaces, tabs, newlines, CR, VT, and FF; signs, leading zeros, and integer boundaries; grouped random inputs of 5, 20, and 100 numbers. NBSP is tested as an invalid separator. |
| Special cases | One element, sorted and reversed inputs, 2–3 elements, INT_MIN/INT_MAX, a rotated sorted list, even values before odd values, and an almost sorted list; disorder at and around 0.2 and 0.5. |
| Permutations | Every ordering of `0..n-1`, up to n=6 normally or n=4 with `--quick`; all four strategies, each with bench enabled. |
| Random inputs and performance | Unique integers from −1,000,000 to 1,000,000; sizes 100 and 500. Each generated input is shared by all four strategies. |
| Memory | A separate Valgrind suite: no arguments, small inputs and 500 elements for each strategy, grouped arguments, and invalid inputs. |
| Norminette | All `.c` and `.h` files recursively under `--source-dir`, including subdirectories. |

For valid input, the tester requires exit status 0, valid instructions with a final newline when output is nonempty, sorted stack A, and empty stack B. Already sorted input must produce zero operations. Without bench, stderr must be empty. Invalid input must produce a positive exit status, empty stdout, and exactly `Error\n` on stderr.

Bench output is compared character by character: disorder percentage and rounding, strategy name, complexity, total operations, and all 11 operation counters. Its expected format is specific to this project.

Valgrind checks memory errors and all leak kinds and must produce a completed XML analysis. It does not run every permutation. Norminette must return status 0, confirm `OK!`, and report no errors.

## 5. Number of tests

With the default sample counts:

| Group | Full | `--quick` |
| --- | ---: | ---: |
| Input and flags | 89 | 89 |
| Grouped numbers | 140 | 140 |
| Special cases | 160 | 160 |
| Permutations | 3492 | 132 |
| Random inputs and performance | 160 | 24 |
| Total without optional tools | **4041** | **545** |
| With Valgrind | **4153** | **657** |

Norminette adds one test per discovered `.c`/`.h` file. External checker calls are counted separately and do not increase the number of test cases. `--samples S` produces `8 × S` cases in the random group. `--quick` does not reduce the separate Valgrind suite.

## 6. Options

| Argument | Default | Meaning |
| --- | --- | --- |
| `binary` | `./push_swap` | Optional program path, e.g. `python3 "42 push swap tester/test.py" ./push_swap`. |
| `--quick` | Disabled | Fewer permutations; defaults to 3 random samples per size instead of 20. |
| `--samples N` | 20 or 3 | Number of random samples separately for n=100 and n=500; must be positive. |
| `--seed N` | 42 | Random seed. The same seed and configuration reproduce the inputs. |
| `--checker PATH` | None | Additional external checker. |
| `--valgrind` | Disabled | Separate memory tests. |
| `--norminette` | Disabled | Source style checks. |
| `--source-dir PATH` | `.` | Norminette source directory, relative to the terminal's working directory. |
| `--timeout S` | 10 | Time limit in seconds for one program or checker invocation. |
| `--valgrind-timeout S` | 60 | Time limit in seconds for one Valgrind case. |
| `--norminette-timeout S` | 60 | Time limit in seconds per source file. |
| `--strict-performance` | Disabled | Enforce the minimum performance requirement for adaptive. |
| `--report PATH` | `test_report.json` | JSON report; overwrites an existing file. The destination directory must exist. |
| `--no-color` | Disabled | Disable colors; the `NO_COLOR` environment variable also works. |
| `--help` | — | Show available options. |

## 7. Results and performance

| Size | Excellent | Good | Minimum | Above limit |
| --- | --- | --- | --- | --- |
| 100 | <700 | 700–1499 | 1500–1999 | ≥2000 |
| 500 | <5500 | 5500–7999 | 8000–11999 | ≥12000 |

The table reports minimum, average, and maximum operation counts. The rating is based on the worst sample. Performance is informational by default. With `--strict-performance`, adaptive using at least 2000 operations for 100 numbers or 12000 for 500 numbers causes FAIL. Simple exceeding these limits does not, by itself, cause FAIL.

The performance group explicitly runs `--adaptive`. Operation without a strategy flag is checked in other groups. A final FAIL with “0 errors” is possible if the only failure is the performance requirement.

| Exit status | Meaning |
| --- | --- |
| 0 | Tests passed, including the performance requirement if enabled. |
| 1 | A test or required performance check failed. |
| 2 | Configuration or report-writing problem. |
| 130 | Interrupted with Ctrl+C; the tester attempts to save partial results. |

Check the exit status immediately after running the tester with `echo $?`.

## 8. Reports and troubleshooting

The terminal displays up to 10 failures. The JSON report contains all detected failures, arguments, commands, stdout, stderr, program exit status, configuration, and measurements. Memory failures may include Valgrind XML; Norminette results are recorded per file.

```bash
python3 -m json.tool test_report.json | less
```

Capture all terminal output while preserving the tester's exit status in Bash:

```bash
set -o pipefail
python3 "42 push swap tester/test.py" --no-color 2>&1 | tee test_python.log
status=${PIPESTATUS[0]}
printf 'Tester exit status: %s\n' "$status"
```

For a failure, copy the command shown after `Powtorz:` (“Repeat”) in the output. For a timeout, increase the relevant limit, e.g. `--valgrind-timeout 120`. If the program is missing, run `make`; for a missing checker, verify its path and executable permissions. Requesting an unavailable Valgrind or Norminette is a configuration error, not a passed check.

PASS applies to the executed cases. Without `--valgrind`, it does not confirm the absence of leaks; without `--norminette`, it does not confirm style compliance. The tester does not simulate malloc failures or cover every possible input. Translating this guide does not change the tester's existing Polish terminal messages.
