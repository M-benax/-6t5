#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

MINISHELL="../minishell"

echo -e "${YELLOW}=== MEMORY LEAK TESTS ===${NC}\n"

run_valgrind_test() {
    local test_name="$1"
    local input="$2"
    
    echo -e "${YELLOW}Testing: $test_name${NC}"
    
    echo -e "$input" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 $MINISHELL > /tmp/valgrind_output 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        echo -e "${GREEN}✓ NO MEMORY LEAKS${NC}"
    else
        echo -e "${RED}✗ MEMORY ISSUES DETECTED${NC}"
        echo "Check /tmp/valgrind_output for details"
    fi
    echo
}

# Test memory management with various scenarios
run_valgrind_test "Basic commands" "echo hello\npwd\nexit"
run_valgrind_test "Environment variables" "export TEST=value\necho \$TEST\nunset TEST\nexit"
run_valgrind_test "Pipes" "echo hello | cat | wc\nexit"
run_valgrind_test "Redirections" "echo test > /tmp/mem_test\ncat < /tmp/mem_test\nexit"
run_valgrind_test "Heredoc" "cat << EOF\nline1\nline2\nEOF\nexit"
run_valgrind_test "Multiple commands" "pwd\necho test\nls /tmp\nexit"
run_valgrind_test "Error handling" "nonexistent_cmd\n| invalid\nexit"

echo -e "${YELLOW}Memory testing complete!${NC}"
rm -f /tmp/mem_test