#!/bin/bash

echo "Testing minishell for memory leaks and double frees..."

# Test 1: Basic commands
echo "Test 1: Basic commands"
echo -e "echo hello\nexit" | ./minishell

echo -e "\n\nTest 2: Environment variables"
echo -e "export TEST=value\necho \$TEST\nexit" | ./minishell

echo -e "\n\nTest 3: Pipes"
echo -e "echo hello | cat\nexit" | ./minishell

echo -e "\n\nTest 4: Redirections"
echo -e "echo test > /tmp/test_file\ncat < /tmp/test_file\nexit" | ./minishell

echo -e "\n\nTest 5: Multiple commands"
echo -e "pwd\nls\necho done\nexit" | ./minishell

echo -e "\n\nTest 6: Invalid commands"
echo -e "nonexistent_command\nexit" | ./minishell

echo -e "\n\nTest 7: Syntax errors"
echo -e "|\nexit" | ./minishell

echo -e "\n\nTest 8: Heredoc"
echo -e "cat << EOF\nhello\nworld\nEOF\nexit" | ./minishell

echo "Memory testing complete!"


