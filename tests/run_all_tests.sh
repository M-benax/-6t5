#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}=== MINISHELL TEST SUITE ===${NC}\n"

# Build the project
echo -e "${YELLOW}Building minishell...${NC}"
cd ..
make re > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Build failed!${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Build successful${NC}\n"

cd tests

# Run execution tests
echo -e "${BLUE}Running execution tests...${NC}"
chmod +x test_execution.sh
./test_execution.sh
EXEC_RESULT=$?

echo -e "\n${BLUE}Running stress tests...${NC}"
chmod +x stress_test.sh
./stress_test.sh

# Build and run GC tests
echo -e "\n${BLUE}Running garbage collector tests...${NC}"
gcc -Wall -Wextra -Werror -g3 -I.. test_gc.c ../memory_management/ft_malloc.c ../utils/utils.c ../utils/utils_2.c -o test_gc 2>/dev/null

if [ $? -eq 0 ]; then
    ./test_gc
    rm -f test_gc
else
    echo -e "${RED}✗ GC test compilation failed${NC}"
fi

# Run memory tests if valgrind is available
if command -v valgrind &> /dev/null; then
    echo -e "\n${BLUE}Running memory leak tests...${NC}"
    chmod +x test_memory.sh
    ./test_memory.sh
else
    echo -e "\n${YELLOW}Valgrind not found, skipping memory leak tests${NC}"
fi

echo -e "\n${BLUE}=== TEST SUITE COMPLETE ===${NC}"

if [ $EXEC_RESULT -eq 0 ]; then
    echo -e "${GREEN}✓ All critical tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed!${NC}"
    exit 1
fi