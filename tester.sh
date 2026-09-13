#!/bin/bash

# ==============================================================================
# PUSH_SWAP AUTOMATED TESTER
# ==============================================================================

# Configuration
SMALL_TESTS_PER_STRATEGY=${SMALL_TESTS_PER_STRATEGY:-222}
STRATEGIES=("--simple" "--medium" "--complex" "")
LARGE_TESTS=${LARGE_TESTS:-55}
LARGE_SIZE=${LARGE_SIZE:-500}

# Additional test blocks
EDGE_TESTS=${EDGE_TESTS:-15}
MEDIUM_LARGE_TESTS=${MEDIUM_LARGE_TESTS:-2222}

TOTAL_PASSED=0
TOTAL_FAILED=0

ERROR_LOG="${ERROR_LOG:-errors.log}"
for key in SMALL_TESTS_PER_STRATEGY LARGE_TESTS LARGE_SIZE EDGE_TESTS MEDIUM_LARGE_TESTS; do
    value=${!key}
    if [[ ! $value =~ ^(0|[1-9][0-9]{0,5})$ ]]; then
        printf 'Invalid configuration: %s=%s\n' "$key" "$value" >&2
        exit 2
    fi
done
if (( LARGE_SIZE < 1 || LARGE_SIZE > 100000 )); then
    echo "LARGE_SIZE must be between 1 and 100000." >&2
    exit 2
fi
> "$ERROR_LOG" || exit 2
TMP_DIR=$(mktemp -d) || exit 2
trap 'rm -rf -- "$TMP_DIR"' EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

# Colors
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
CYAN="\033[0;36m"
BLUE="\033[1;34m"
MAGENTA="\033[1;35m"
WHITE="\033[1;37m"
RESET="\033[0m"

# ==============================================================================
# CHECK BINARIES
# ==============================================================================

if [ ! -x "./push_swap" ]; then
    echo -e "${RED}Error: ./push_swap binary not found or not executable!${RESET}"
    echo "Compile first, for example: make"
    exit 1
fi

if [ -x "./checker" ]; then
    CHECKER="./checker"
elif [ -x "./checker_linux" ]; then
    CHECKER="./checker_linux"
else
    echo -e "${RED}Error: checker or checker_linux not found or not executable!${RESET}"
    echo "Put checker in this directory and run:"
    echo "chmod +x checker"
    echo "or:"
    echo "chmod +x checker_linux"
    exit 1
fi

echo "=================================================="
echo "          PUSH_SWAP AUTOMATED TESTER              "
echo "=================================================="
echo "Checker: $CHECKER"

# ==============================================================================
# HELPER: GENERATE UNIQUE RANDOM NUMBERS
# ==============================================================================

generate_numbers()
{
    local size="$1"
    local min="$2"
    local max="$3"
    local seed="$RANDOM"

    awk -v size="$size" -v min="$min" -v max="$max" -v seed="$seed" 'BEGIN {
        srand(seed);
        count = 0;
        range = max - min + 1;

        while (count < size) {
            num = int(rand() * range) + min;

            if (!(num in picked)) {
                picked[num] = 1;
                count++;
                printf "%d ", num;
            }
        }
    }'
}

# ==============================================================================
# HELPER: RUN PUSH_SWAP
# ==============================================================================

run_push_swap()
{
    local strategy="$1"
    shift

    if [ -z "$strategy" ]; then
        ./push_swap "$@"
    else
        ./push_swap "$strategy" "$@"
    fi
}

# ==============================================================================
# HELPER: RUN CHECKER
# ==============================================================================

# Keep stdout in a file: command substitution would strip trailing newlines.
# The checker receives the exact bytes emitted by push_swap, including zero bytes.
evaluate_case()
{
    local strategy="$1"
    shift
    CHECKER_RESULT=""
    CHECKER_STATUS="not run"
    FAILURE_REASON=""
    : > "$TMP_DIR/checker.err"
    run_push_swap "$strategy" "$@" > "$TMP_DIR/ops" 2> "$TMP_DIR/push.err"
    PUSH_STATUS=$?
    OPS=$(cat "$TMP_DIR/ops")
    OP_COUNT=$(awk 'END { print NR + 0 }' "$TMP_DIR/ops")

    if (( $# > 0 )); then
        "$CHECKER" "$@" < "$TMP_DIR/ops" > "$TMP_DIR/checker.out" 2> "$TMP_DIR/checker.err"
        CHECKER_STATUS=$?
        CHECKER_RESULT=$(cat "$TMP_DIR/checker.out")
    elif [[ ! -s "$TMP_DIR/ops" ]]; then
        CHECKER_RESULT="OK"
    else
        CHECKER_RESULT="KO"
    fi

    if (( PUSH_STATUS != 0 )); then
        FAILURE_REASON="push_swap exited with status $PUSH_STATUS"
    elif [[ -s "$TMP_DIR/push.err" ]]; then
        FAILURE_REASON="Unexpected push_swap stderr for valid input"
    elif (( $# == 0 )) && [[ -s "$TMP_DIR/ops" ]]; then
        FAILURE_REASON="Expected empty stdout for no arguments"
    elif (( $# > 0 )) && [[ $CHECKER_STATUS != 0 ]]; then
        FAILURE_REASON="Checker exited with status $CHECKER_STATUS"
    elif [[ -s "$TMP_DIR/checker.err" ]]; then
        FAILURE_REASON="Unexpected checker stderr"
    elif [[ $CHECKER_RESULT != OK ]]; then
        FAILURE_REASON="Checker did not return OK"
    fi
    [[ -z $FAILURE_REASON ]] || CHECKER_RESULT="FAIL: $FAILURE_REASON"
}

# ==============================================================================
# HELPER: SAVE FAILED TEST
# ==============================================================================

log_failure()
{
    local section="$1"
    local test_no="$2"
    local size="$3"
    local strategy="$4"
    local arg="$5"
    local checker_result="$6"
    local ops="$7"

    {
        echo "=================================================="
        echo "TEST FAILED"
        echo "Section: $section"
        echo "Test: $test_no"
        echo "Size: $size"
        if [ -n "$strategy" ]; then
            echo "Strategy: $strategy"
            echo "Command: ./push_swap $strategy $arg"
        else
            echo "Strategy: Adaptive (No Flag)"
            echo "Command: ./push_swap $arg"
        fi
        echo "ARG: $arg"
        echo "Reason: $FAILURE_REASON"
        echo "push_swap exit: $PUSH_STATUS"
        echo "Checker exit: $CHECKER_STATUS"
        echo "Checker: $checker_result"
        echo "push_swap stderr:"
        cat "$TMP_DIR/push.err"
        echo "Checker stderr:"
        cat "$TMP_DIR/checker.err"
        echo "Raw checker stdout:"
        if [[ $CHECKER_STATUS != 'not run' ]]; then
            cat "$TMP_DIR/checker.out"
        fi
        echo "Operation bytes: $(wc -c < "$TMP_DIR/ops")"
        echo "Operations:"
        cat "$TMP_DIR/ops"
        echo
    } >> "$ERROR_LOG"
}

# Deterministic regressions: sorted input must work without any operations.
REGRESSION_CASES=("-17" "-35 23" "-568 331 485 875" "-674 -115 212 560 908" "2 1" "3 1 2")
REGRESSION_TOTAL=$(( ${#REGRESSION_CASES[@]} * ${#STRATEGIES[@]} ))
echo -e "\n${MAGENTA}>>> REGRESSION: EMPTY OUTPUT AND BASIC SORTING <<<${RESET}"
reg_no=0
for strategy in "${STRATEGIES[@]}"; do
    for ARG in "${REGRESSION_CASES[@]}"; do
        ((reg_no++))
        read -r -a ARGS <<< "$ARG"
        evaluate_case "$strategy" "${ARGS[@]}"
        if [[ $CHECKER_RESULT == OK ]]; then
            ((TOTAL_PASSED++))
            printf '  [%d/%d] %s | %s: OK (%s ops)\n' "$reg_no" "$REGRESSION_TOTAL" "${strategy:-Adaptive}" "$ARG" "$OP_COUNT"
        else
            ((TOTAL_FAILED++))
            printf '  [%d/%d] FAIL: %s\n' "$reg_no" "$REGRESSION_TOTAL" "$FAILURE_REASON"
            log_failure REGRESSION "$reg_no/$REGRESSION_TOTAL" "${#ARGS[@]}" "$strategy" "$ARG" "$CHECKER_RESULT" "$OPS"
        fi
    done
done

# ==============================================================================
# SECTION 1: EDGE CASES (SIZES 0 TO 3)
# ==============================================================================

echo -e "\n${MAGENTA}>>> SECTION 1: EDGE CASES (SIZES 0 TO 3) <<<${RESET}"
echo "--------------------------------------------------"

edge_passed=0

for ((i=1; i<=EDGE_TESTS; i++)); do
    SIZE=$((RANDOM % 4))

    ARG=$(generate_numbers "$SIZE" -50 49)
    ARG=$(echo "$ARG" | xargs)

    ARGS=()
    read -r -a ARGS <<< "$ARG"
    evaluate_case "" "${ARGS[@]}"

    if [ "$CHECKER_RESULT" = "OK" ]; then
        echo -e "  [Edge Test $i/$EDGE_TESTS] Size $SIZE: ${GREEN}OK${RESET} ($OP_COUNT ops)"
        ((edge_passed++))
        ((TOTAL_PASSED++))
    else
        echo -e "  [Edge Test $i/$EDGE_TESTS] Size $SIZE: ${RED}KO / Error${RESET} -> ARG=\"$ARG\""
        log_failure "EDGE" "$i/$EDGE_TESTS" "$SIZE" "" "$ARG" "$CHECKER_RESULT" "$OPS"
        ((TOTAL_FAILED++))
    fi
done

# ==============================================================================
# SECTION 2: SMALL STACK TESTING (SIZES 4 & 5)
# ==============================================================================

echo -e "\n${MAGENTA}>>> SECTION 2: SMALL STACK TESTING (SIZES 4 & 5) <<<${RESET}"

for strategy in "${STRATEGIES[@]}"; do
    if [ -z "$strategy" ]; then
        DISPLAY_NAME="Adaptive (No Flag)"
    else
        DISPLAY_NAME="$strategy"
    fi

    echo -e "\nRunning $SMALL_TESTS_PER_STRATEGY tests for strategy: ${BLUE}$DISPLAY_NAME${RESET}"
    echo "--------------------------------------------------"

    strategy_passed=0

    for ((i=1; i<=SMALL_TESTS_PER_STRATEGY; i++)); do
        SIZE=$((RANDOM % 2 + 4))

        ARG=$(generate_numbers "$SIZE" -1000 999)
        ARG=$(echo "$ARG" | xargs)
        read -r -a ARGS <<< "$ARG"

        evaluate_case "$strategy" "${ARGS[@]}"

        if [ "$CHECKER_RESULT" = "OK" ]; then
            echo -e "  [Test $i/$SMALL_TESTS_PER_STRATEGY] Size $SIZE: ${GREEN}OK${RESET} ($OP_COUNT ops)"
            ((strategy_passed++))
            ((TOTAL_PASSED++))
        else
            echo -e "  [Test $i/$SMALL_TESTS_PER_STRATEGY] Size $SIZE: ${RED}KO / Error${RESET} -> ARG=\"$ARG\""

            if [ -z "$strategy" ]; then
                echo "  Executed: ./push_swap \$ARG"
            else
                echo "  Executed: ./push_swap $strategy \$ARG"
            fi

            log_failure "SMALL" "$i/$SMALL_TESTS_PER_STRATEGY" "$SIZE" "$strategy" "$ARG" "$CHECKER_RESULT" "$OPS"
            ((TOTAL_FAILED++))
        fi
    done
done

# ==============================================================================
# SECTION 3: ADAPTIVE STRESS TEST (FIXED SIZE)
# ==============================================================================

echo -e "\n${MAGENTA}>>> SECTION 3: ADAPTIVE STRATEGY STRESS TEST (SIZE $LARGE_SIZE) <<<${RESET}"
echo "--------------------------------------------------"

large_passed=0

for ((i=1; i<=LARGE_TESTS; i++)); do
    ARG=$(generate_numbers "$LARGE_SIZE" -50000 49999)
    ARG=$(echo "$ARG" | xargs)
    read -r -a ARGS <<< "$ARG"

    evaluate_case "" "${ARGS[@]}"

    if [ "$CHECKER_RESULT" = "OK" ]; then
        ((large_passed++))
        ((TOTAL_PASSED++))

        if [ "$LARGE_SIZE" -eq 500 ]; then
            if [ "$OP_COUNT" -lt 5500 ]; then
                PERF_RATING="${GREEN}Excellent${RESET}"
            elif [ "$OP_COUNT" -lt 8000 ]; then
                PERF_RATING="${CYAN}Good${RESET}"
            elif [ "$OP_COUNT" -lt 12000 ]; then
                PERF_RATING="${YELLOW}Pass${RESET}"
            else
                PERF_RATING="${RED}Above target (informational)${RESET}"
            fi
        elif [ "$LARGE_SIZE" -eq 100 ]; then
            if [ "$OP_COUNT" -lt 700 ]; then
                PERF_RATING="${GREEN}Excellent${RESET}"
            elif [ "$OP_COUNT" -lt 1500 ]; then
                PERF_RATING="${CYAN}Good${RESET}"
            elif [ "$OP_COUNT" -lt 2000 ]; then
                PERF_RATING="${YELLOW}Pass${RESET}"
            else
                PERF_RATING="${RED}Above target (informational)${RESET}"
            fi
        fi

        if [[ $LARGE_SIZE != 100 && $LARGE_SIZE != 500 ]]; then
            PERF_RATING="Not rated for this size"
        fi
        echo -e "  [Large Test $i/$LARGE_TESTS]: ${GREEN}OK${RESET} ($OP_COUNT ops) -> $PERF_RATING"
    else
        if [[ $LARGE_SIZE != 100 && $LARGE_SIZE != 500 ]]; then
            PERF_RATING="Not rated for this size"
        fi
        echo -e "  [Large Test $i/$LARGE_TESTS]: ${RED}KO / Error${RESET} -> ARG=\"$ARG\""
        log_failure "LARGE" "$i/$LARGE_TESTS" "$LARGE_SIZE" "" "$ARG" "$CHECKER_RESULT" "$OPS"
        ((TOTAL_FAILED++))
    fi
done

# ==============================================================================
# SECTION 4: MEDIUM-LARGE ADAPTIVE STRESS TEST (SIZES 100 TO 500)
# ==============================================================================

echo -e "\n${MAGENTA}>>> SECTION 4: ADAPTIVE MIXED STRESS TEST (SIZES 100 TO 500) <<<${RESET}"
echo "--------------------------------------------------"

ml_passed=0

for ((i=1; i<=MEDIUM_LARGE_TESTS; i++)); do
    SIZE=$((RANDOM % 401 + 100))

    ARG=$(generate_numbers "$SIZE" -100000 99999)
    ARG=$(echo "$ARG" | xargs)
    read -r -a ARGS <<< "$ARG"

    evaluate_case "" "${ARGS[@]}"

    if [ "$CHECKER_RESULT" = "OK" ]; then
        ((ml_passed++))
        ((TOTAL_PASSED++))

        if [ "$SIZE" -eq 500 ]; then
            if [ "$OP_COUNT" -lt 5500 ]; then
                PERF_RATING="${GREEN}Excellent${RESET}"
            elif [ "$OP_COUNT" -lt 8000 ]; then
                PERF_RATING="${CYAN}Good${RESET}"
            elif [ "$OP_COUNT" -lt 12000 ]; then
                PERF_RATING="${YELLOW}Pass${RESET}"
            else
                PERF_RATING="${YELLOW}Above target (informational)${RESET}"
            fi
        else
            PERF_RATING="${GREEN}OK${RESET}"
        fi

        echo -e "  [ML Test $i/$MEDIUM_LARGE_TESTS] Size $SIZE: ${GREEN}OK${RESET} ($OP_COUNT ops) -> $PERF_RATING"
    else
        echo -e "  [ML Test $i/$MEDIUM_LARGE_TESTS] Size $SIZE: ${RED}KO / Error${RESET} -> ARG=\"$ARG\""
        log_failure "MEDIUM-LARGE" "$i/$MEDIUM_LARGE_TESTS" "$SIZE" "" "$ARG" "$CHECKER_RESULT" "$OPS"
        ((TOTAL_FAILED++))
    fi
done

# ==============================================================================
# FINAL SUMMARY
# ==============================================================================

TOTAL_PLANNED=$((REGRESSION_TOTAL + EDGE_TESTS + SMALL_TESTS_PER_STRATEGY * ${#STRATEGIES[@]} + LARGE_TESTS + MEDIUM_LARGE_TESTS))

TOTAL_EXECUTED=$((TOTAL_PASSED + TOTAL_FAILED))

echo -e "\n=================================================="
echo "                 FINAL SUMMARY                    "
echo "=================================================="
echo -e "Total Executed:   ${WHITE}$TOTAL_EXECUTED${RESET} / $TOTAL_PLANNED"
echo -e "Total Successful: ${GREEN}$TOTAL_PASSED${RESET}"
echo -e "Failed tests log: $ERROR_LOG"
echo "Performance ratings are informational; exit status reflects correctness."

if [ "$TOTAL_FAILED" -gt 0 ]; then
    echo -e "Total Failures:   ${RED}$TOTAL_FAILED${RESET}"
    exit 1
else
    echo -e "All test profiles evaluated as ${GREEN}SUCCESSFUL${RESET}!"
    exit 0
fi
