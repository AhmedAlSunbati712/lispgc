#!/bin/sh
# Usage: tests/run_tests.sh <path-to-binary>

# Make sure a path to the binary file is passed
if [ $# -lt 1 ]; then
    echo "Error: missing path to binary file" >&2
    echo "Usage: $0 <path-to-binary>" >&2
    exit 1
fi

# Make sure the path exists and the file is executable
if [ ! -x "$1" ]; then
    echo "Error: binary file not found or not executable: $1" >&2
    exit 1
fi

BIN="$1"
fail=0 # number of tests that fail
pass=0 # number of tests that pass

# Temporary files to hold stdout and stderr (reused by every test, and
# removed when the script exits, however it exits)
out="$(mktemp)"
err="$(mktemp)"
trap 'rm -f "$out" "$err"' EXIT

for f in tests/cases/*.scm; do
    # If nothing matches, the glob stays as the literal pattern
    if [ ! -e "$f" ]; then
        echo "No test cases found in tests/cases/"
        break
    fi

    name="$(basename "$f" .scm)"
    expected="${f%.scm}.expected"

    if [ ! -f "$expected" ]; then
        echo "FAIL $name (missing $expected)"
        fail=$((fail + 1))
        continue
    fi

    # run the binary and redirect the stdout to out and the stderr to err
    "$BIN" "$f" >"$out" 2>"$err"
    status=$? # capture the status

    if [ "$status" -ne 0 ]; then
        echo "FAIL $name (exit status $status)"
        if [ -s "$err" ]; then
            echo "    --- stderr (first 20 lines) ---"
            head -n 20 "$err" | sed 's/^/    /'
        fi
        fail=$((fail + 1))
        continue
    fi

    if diff -u "$expected" "$out" >/dev/null; then
        echo "PASS $name"
        pass=$((pass + 1))
    else
        echo "FAIL $name (output differs from expected)"
        echo "    --- diff (expected vs actual) ---"
        diff -u "$expected" "$out" | sed 's/^/    /'
        if [ -s "$err" ]; then
            echo "    --- stderr (first 20 lines) ---"
            head -n 20 "$err" | sed 's/^/    /'
        fi
        fail=$((fail + 1))
    fi
done

echo "$pass passed, $fail failed"

# non-zero exit if any of the cases failed
if [ "$fail" -ne 0 ]; then
    exit 1
fi
exit 0
