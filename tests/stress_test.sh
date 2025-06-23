#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

MINISHELL="../minishell"

echo -e "${YELLOW}=== STRESS TESTS ===${NC}\n"

# Test 1: Many allocations
echo -e "${YELLOW}Test 1: Multiple environment variables${NC}"
{
    for i in {1..50}; do
        echo "export VAR$i=value$i"
    done
    echo "env | wc -l"
    echo "exit"
} | timeout 10s $MINISHELL > /dev/null 2>&1

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Multiple env vars test passed${NC}"
else
    echo -e "${RED}✗ Multiple env vars test failed${NC}"
fi

# Test 2: Long pipes
echo -e "\n${YELLOW}Test 2: Long pipeline${NC}"
{
    echo -n "echo hello"
    for i in {1..10}; do
        echo -n " | cat"
    done
    echo
    echo "exit"
} | timeout 10s $MINISHELL > /dev/null 2>&1

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Long pipeline test passed${NC}"
else
    echo -e "${RED}✗ Long pipeline test failed${NC}"
fi

# Test 3: Many redirections
echo -e "\n${YELLOW}Test 3: Multiple redirections${NC}"
{
    for i in {1..20}; do
        echo "echo test$i > /tmp/stress_test_$i"
    done
    echo "cat /tmp/stress_test_* | wc -l"
    echo "exit"
} | timeout 10s $MINISHELL > /dev/null 2>&1

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Multiple redirections test passed${NC}"
else
    echo -e "${RED}✗ Multiple redirections test failed${NC}"
fi

# Test 4: Large heredoc
echo -e "\n${YELLOW}Test 4: Large heredoc${NC}"
{
    echo "cat << EOF"
    for i in {1..100}; do
        echo "Line $i with some content to make it longer"
    done
    echo "EOF"
    echo "exit"
} | timeout 10s $MINISHELL > /dev/null 2>&1

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Large heredoc test passed${NC}"
else
    echo -e "${RED}✗ Large heredoc test failed${NC}"
fi

# Cleanup
rm -f /tmp/stress_test_*

echo -e "\n${YELLOW}Stress testing complete!${NC}"