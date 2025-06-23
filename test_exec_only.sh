#!/bin/bash

MINISHELL="./minishell"
PASSED=0
FAILED=0

test_cmd() {
    echo -n "Testing: $1 ... "
    echo -e "$2" | timeout 5s $MINISHELL > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo "PASS"
        ((PASSED++))
    else
        echo "FAIL"
        ((FAILED++))
    fi
}

echo "=== EXECUTION TESTS ==="

test_cmd "echo command" "echo hello\nexit"
test_cmd "pwd command" "pwd\nexit"
test_cmd "pipe" "echo hello | cat\nexit"
test_cmd "redirection" "echo test > /tmp/out\ncat /tmp/out\nexit"
test_cmd "export" "export VAR=value\necho \$VAR\nexit"
test_cmd "cd" "cd /tmp\npwd\nexit"

echo
echo "PASSED: $PASSED"
echo "FAILED: $FAILED"

if [ $FAILED -eq 0 ]; then
    echo "ALL TESTS PASSED!"
else
    echo "SOME TESTS FAILED!"
fi