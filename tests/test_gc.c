#include "../minishell.h"
#include <assert.h>

void test_gc_malloc_free() {
    t_data data = {0};
    
    // Test basic allocation
    void *ptr1 = gc_malloc(100, &data);
    assert(ptr1 != NULL);
    assert(data.g_gc_list != NULL);
    
    void *ptr2 = gc_malloc(200, &data);
    assert(ptr2 != NULL);
    
    // Test individual free
    gc_free_ptr(ptr1, &data);
    
    // Test free all
    gc_free_all(&data);
    assert(data.g_gc_list == NULL);
    
    printf("✓ GC malloc/free tests passed\n");
}

void test_ft_strdup() {
    t_data data = {0};
    
    char *dup = ft_strdup("hello world", &data);
    assert(dup != NULL);
    assert(strcmp(dup, "hello world") == 0);
    
    gc_free_all(&data);
    printf("✓ ft_strdup test passed\n");
}

void test_ft_strndup() {
    t_data data = {0};
    
    char *dup = ft_strndup("hello world", 5, &data);
    assert(dup != NULL);
    assert(strcmp(dup, "hello") == 0);
    assert(strlen(dup) == 5);
    
    gc_free_all(&data);
    printf("✓ ft_strndup test passed\n");
}

void test_new_node() {
    t_data data = {0};
    
    t_comand *node = new_node("test", 1, &data);
    assert(node != NULL);
    assert(strcmp(node->word, "test") == 0);
    assert(node->quoted == 1);
    assert(node->next == NULL);
    
    gc_free_all(&data);
    printf("✓ new_node test passed\n");
}

void test_gc_free_array() {
    t_data data = {0};
    
    char **array = gc_malloc(3 * sizeof(char*), &data);
    array[0] = ft_strdup("first", &data);
    array[1] = ft_strdup("second", &data);
    array[2] = NULL;
    
    gc_free_array(array, &data);
    
    printf("✓ gc_free_array test passed\n");
}

int main() {
    printf("=== GARBAGE COLLECTOR TESTS ===\n\n");
    
    test_gc_malloc_free();
    test_ft_strdup();
    test_ft_strndup();
    test_new_node();
    test_gc_free_array();
    
    printf("\n✓ All GC tests passed!\n");
    return 0;
}