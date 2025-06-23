#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

MINISHELL="../minishell"
PASSED=0
FAILED=0

echo -e "${YELLOW}=== MINISHELL EXECUTION TESTS ===${NC}\n"

run_test() {
    local test_name="$1"
    local input="$2"
    local expected_exit="$3"
    
    echo -e "${YELLOW}Testing: $test_name${NC}"
    
    echo -e "$input" | timeout 5s $MINISHELL > /tmp/minishell_output 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq $expected_exit ] || [ $expected_exit -eq -1 ]; then
        echo -e "${GREEN}✓ PASSED${NC}"
        ((PASSED++))
    else
        echo -e "${RED}✗ FAILED (exit code: $exit_code, expected: $expected_exit)${NC}"
        ((FAILED++))
    fi
    echo
}

# Basic commands
run_test "Basic echo command" "echo hello world\nexit" 0
run_test "PWD command" "pwd\nexit" 0
run_test "CD command" "cd /tmp\npwd\nexit" 0

# Environment variables
run_test "Export and echo variable" "export TEST=hello\necho \$TEST\nexit" 0
run_test "Unset variable" "export TEST=hello\nunset TEST\necho \$TEST\nexit" 0

# Pipes
run_test "Simple pipe" "echo hello | cat\nexit" 0
run_test "Multiple pipes" "echo hello | cat | cat\nexit" 0

# Redirections
run_test "Output redirection" "echo test > /tmp/test_out\ncat /tmp/test_out\nexit" 0
run_test "Input redirection" "echo content > /tmp/test_in\ncat < /tmp/test_in\nexit" 0
run_test "Append redirection" "echo line1 > /tmp/test_append\necho line2 >> /tmp/test_append\ncat /tmp/test_append\nexit" 0

# Heredoc
run_test "Heredoc" "cat << EOF\nhello\nworld\nEOF\nexit" 0

# Error handling
run_test "Command not found" "nonexistent_command_12345\nexit" 0
run_test "Syntax error - pipe at start" "| echo hello\nexit" 0

# Exit codes
run_test "Exit with code" "exit 42" 42

# Complex commands
run_test "Complex pipe with redirection" "echo hello | cat > /tmp/complex_test\ncat /tmp/complex_test\nexit" 0

# Quotes handling
run_test "Single quotes" "echo 'hello world'\nexit" 0
run_test "Double quotes" "echo \"hello world\"\nexit" 0

echo -e "${YELLOW}=== TEST SUMMARY ===${NC}"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"

rm -f /tmp/test_out /tmp/test_in /tmp/test_append /tmp/complex_test /tmp/minishell_output

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi