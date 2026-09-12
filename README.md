_This project has been created as part of the 42 curriculum by dswietoc, ryakubov._

# Push_swap

Because Swap_push doesn't feel as natural

## Description

`Push_swap` is a 42 school algorithmic project. The goal is to sort a stack of
integers (`stack a`) using a second, initially empty stack (`stack b`) and a
restricted set of operations, while aiming to keep the number of instructions as low as possible.

Given a list of unique integers passed as program arguments, the program must:

1. Validate the input (only integers, no duplicates, respecting `int` bounds).
2. Sort `stack a` in ascending order (top of stack = smallest value) using
   only `stack b` as auxiliary storage.
3. Print, on `stdout`, the exact sequence of operations used to reach the
   sorted state.
4. Print `Error\n` on `stderr` (and exit with a non-zero status) for any
   invalid input, without printing anything on `stdout`.

The allowed operations are:

| Operation | Effect                                             |
| --------- | -------------------------------------------------- |
| `sa`      | Swap the first 2 elements at the top of stack `a`. |
| `sb`      | Swap the first 2 elements at the top of stack `b`. |
| `ss`      | `sa` and `sb` at the same time.                    |
| `pa`      | Push the top element of `b` onto `a`.              |
| `pb`      | Push the top element of `a` onto `b`.              |
| `ra`      | Rotate `a` up (first element becomes last).        |
| `rb`      | Rotate `b` up (first element becomes last).        |
| `rr`      | `ra` and `rb` at the same time.                    |
| `rra`     | Reverse-rotate `a` (last element becomes first).   |
| `rrb`     | Reverse-rotate `b` (last element becomes first).   |
| `rrr`     | `rra` and `rrb` at the same time.                  |

This implementation is **adaptive**: it measures how "disordered" the input
is and automatically selects one of the three sorting strategies according to
predefined heuristic thresholds, with the goal of keeping the operation count
low for different input distributions.

## Instructions

### Compilation

```bash
make            # builds the push_swap binary
make clean      # removes object files
make fclean     # removes object files and the binary
make re         # fclean + all
```

The `Makefile` compiles all sources listed below with
`-Wall -Wextra -Werror`, using the `cc` compiler, and links them into a
single `Push_swap` executable.

### Usage

```bash
./push_swap [OPTIONS] [ARGUMENTS]
```

`ARGUMENTS` can be given either as several separate arguments or as a single
quoted string of space-separated integers:

```bash
./push_swap 2 1 3 6 5 8
./push_swap "2 1 3 6 5 8"
```

With no arguments, the program does nothing and exits with status `0`.
On any invalid input (non-integer token, duplicate value, integer overflow),
the program prints `Error\n` on `stderr` and exits with status `1`.

### Optional flags (extensions on top of the mandatory subject)

| Flag                   | Effect                                                                                                                |
| ---------------------- | --------------------------------------------------------------------------------------------------------------------- |
| `--simple`             | Force the O(n²) simple strategy.                                                                                      |
| `--medium`             | Force the O(n·√n) chunk-based strategy.                                                                               |
| `--complex`            | Force the O(n·log n) radix-based strategy.                                                                            |
| `--adaptive` (default) | Pick a strategy automatically based on input disorder.                                                                |
| `--bench`              | Print, on `stderr`, a benchmark summary (disorder %, strategy used, complexity class, and a per-operation breakdown). |

These flags are additive: without a strategy flag, the program uses the adaptive strategy and produces
the same type of standard push_swap instruction output on stdout, without
benchmark information unless `--bench` is explicitly enabled.

### Checking the result

The result can be verified with the standard 42 `checker` program or with any
community checker/visualizer, by piping the operations produced on `stdout`.

## Algorithms — explanation and justification

All algorithms operate on a singly linked list (`t_stack`) and only ever
allocate `O(n)` nodes total (one per input integer); no algorithm allocates
additional nodes during sorting, so **space complexity is `O(n)`** across the
board. Below, `n` is the number of elements to sort.

### Disorder metric

Computes the fraction of _inverted pairs_ in `stack a`.
This yields a value in `[0, 1]`:

- `0.0` → the stack is already sorted.
- close to `1.0` → the stack is close to reverse-sorted (maximally disordered).

This metric is the input to the adaptive strategy selector.

### Simple sort — O(n²)

**Technique:** repeatedly find the minimum remaining value in `a`, rotate it to the top with the cheaper of `ra`/`rra`
depending on which half of the stack it is in, then `pb` it onto `b`. Once
`a` is empty, `pa` everything back — since values were pushed to `b` from
smallest to largest, they come back in sorted order.

**Justification:** this is the simplest correct algorithm and requires no
extra bookkeeping (no indices/ranks). Each element requires at most O(n) rotations in the current stack, and the
algorithm performs O(n) push operations. Therefore, the overall push_swap
operation count is O(n²).

### Medium sort — chunk-based strategy

**Technique:** the algorithm divides the ranked value range into approximately
`√n` chunks. For each chunk, it scans stack `a` and pushes matching elements
to stack `b`. Elements from the lower part of a chunk may be rotated in `b`
to improve its internal ordering. After all chunks have been processed, the
algorithm repeatedly brings the maximum element of `b` to the top and pushes
it back to `a`.

**Justification:** square-root chunking reduces the amount of unrelated data
that has to be considered while processing each value range and provides a
practical compromise between the simple quadratic strategy and the radix-based
strategy. The strategy uses approximately √n chunks as a practical compromise between
the simple and radix-based approaches. The implementation performs additional
linear scans when calculating ranks and locating the maximum element in stack
b, so the chunk size alone does not provide a strict O(n·√n) CPU-time bound
for the complete implementation. The complexity should therefore be understood
as a description of the chunk-based strategy rather than a formal upper bound
for every operation performed by the implementation.

### Complex sort — radix-based strategy

**Technique:** LSD (least-significant-bit) radix sort, adapted to the
push_swap operation set.

1. Converts each value into its **rank** (`0` to `n-1`),
   so radix sort can operate on ranks instead of raw (possibly negative)
   integers.
2. For each bit position `b` from `0` up to `⌈log2(n)⌉ - 1`:
   - every element of `a` is inspected: if bit `b` of its rank is `0` it is
     pushed to `b` (`pb`), otherwise it is rotated to the back of `a` (`ra`);
   - once `a` is empty, everything is pushed back with `pa` — since `b`
     received "bit = 0" elements first, this preserves the correct relative
     order for the next, more significant bit.

**Justification:** the radix phase performs `O(log n)` passes, and each pass
processes every element once, resulting in `O(n·log n)` push_swap operations.
The current rank-assignment helper uses nested scans and therefore takes
`O(n²)` CPU time before the radix passes begin. Consequently, `O(n·log n)`
describes the operation count of the radix phase, while the complete
implementation has an `O(n²)` CPU-time component due to rank assignment.

### Adaptive strategy

The adaptive strategy automatically selects the most suitable sorting algorithm based on the input disorder:

- Disorder < 20% → Simple O(n²)
- Disorder 20-49% → Medium O(n√n)
- Disorder ≥ 50% → Complex O(n log n)

This allows push_swap to choose an efficient strategy depending on how ordered the input data is.

**Threshold rationale**

The thresholds were selected as practical heuristics rather than mathematically
optimal cut-off points. Inputs with less than 20% disorder are sufficiently
close to sorted order that the simple strategy can exploit their structure
without requiring the additional bookkeeping of the other strategies.

Between 20% and 50% disorder, the medium chunk-based strategy provides a
compromise between the simplicity of the quadratic strategy and the more
systematic radix approach.

For disorder of 50% or more, the input is sufficiently disordered that the
radix-based strategy is preferred because its push_swap operation count grows
more predictably with the number of elements.

These thresholds are heuristic design choices and can be adjusted after
benchmarking different input sizes and distributions.

### Complexity summary

| Strategy | Push_swap operation class | Internal CPU-time considerations                | Space |
| -------- | ------------------------- | ----------------------------------------------- | ----- |
| Simple   | O(n²)                     | O(n²)                                           | O(n)  |
| Medium   | Chunk-based strategy      | Additional linear rank/max scans                | O(n)  |
| Complex  | O(n·log n) radix phase    | O(n²) rank assignment + O(n·log n) radix passes | O(n)  |
| Adaptive | Selects one of the above  | O(n²) disorder calculation + selected strategy  | O(n)  |

## Contributions

| Learner      | Areas of contribution                                                                          |
| ------------ | ---------------------------------------------------------------------------------------------- |
| **dswietoc** | Stack swap operations, simple and complex implementations, disorder metric and benchmark mode. |
| **ryakubov** | Input validation, error handling, medium algorithm and project documentation.                  |

Both learners participated in code review, debugging, and testing of the entire codebase.

## Resources

### Classic references

- 42 `Push_swap` subject PDF (official assignment sheet).
- [Sorting algorithm complexity overview — Wikipedia](https://en.wikipedia.org/wiki/Sorting_algorithm)

### Use of AI

An AI assistant was used during this project for **Documentation**: structuring and formatting this `README.md` file.
