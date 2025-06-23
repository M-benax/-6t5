# Memory Issues Report - Minishell Project

## Critical Memory Issues Found

### 1. MEMORY LEAK - main/minishell.c:23
**Location:** Line 23
**Issue:** `data->tenv = gc_malloc(sizeof(t_env), data);` is immediately overwritten
**Code:**
```c
data->tenv = gc_malloc(sizeof(t_env), data);  // LEAKED - never freed
data->tenv = init_env(env, data);             // Overwrites previous allocation
```
**Impact:** Memory leak of sizeof(t_env) bytes on every program start

### 2. MEMORY LEAK - main/minishell.c:21
**Location:** Line 21
**Issue:** Main data structure allocated with malloc() but never freed
**Code:**
```c
data = malloc(sizeof(t_data));  // NEVER FREED
```
**Impact:** Memory leak of sizeof(t_data) bytes on every program run

### 3. MEMORY LEAK - bilds/export.c:35
**Location:** Lines 32-36
**Issue:** `val` pointer not freed when creating first env node
**Code:**
```c
if (!tmp)
{
    *envp = new_env_node(var, val, data);
    gc_free_ptr(var, data);
    return;  // val is NOT freed here - MEMORY LEAK
}
```
**Impact:** Memory leak when first environment variable is set

### 4. POTENTIAL DOUBLE FREE - memory_management/ft_malloc.c:94
**Location:** Lines 89-95
**Issue:** gc_free_all() called before exit, but pointers remain in list
**Code:**
```c
if (!node)
{
    gc_free_all(data);  // Frees all pointers
    printf("malloc failed\n");
    exit(EXIT_FAILURE);  // Exit without clearing list - potential double free
}
```
**Impact:** Potential double free if gc_free_all() called again

### 5. MEMORY LEAK - bilds/expand.c:130-140
**Location:** Lines 130-140
**Issue:** Only frees cmd->args[index] but not other array elements
**Code:**
```c
if (new_argc == 0)
{
    gc_free_ptr(cmd->args[index], data);  // Only frees one element
    gc_free_ptr(cmd->args, data);         // Frees array but not other elements
    gc_free_ptr(cmd->quoted_flags, data);
    // OTHER cmd->args[0], cmd->args[1], etc. NOT FREED - MEMORY LEAK
}
```
**Impact:** Memory leak of all cmd->args elements except [index]

### 6. LOGIC BUG - Export Command Variable Expansion
**Location:** bilds/export.c:handel_export()
**Issue:** Variable expansion happens after validation, should be before
**Example:** `export $a="value"` fails validation because `$a` contains `$`
**Impact:** Export command doesn't work with variable expansion

## Memory Issue Locations Summary:
- **main/minishell.c**: Lines 21, 23 (2 leaks)
- **bilds/export.c**: Line 35 (1 leak)  
- **memory_management/ft_malloc.c**: Line 94 (1 potential double free)
- **bilds/expand.c**: Lines 130-140 (1 leak)
- **bilds/export.c**: handel_export() function (1 logic bug)

## Total Issues: 5 Memory Issues + 1 Logic Bug