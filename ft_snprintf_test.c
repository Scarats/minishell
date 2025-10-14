/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf_test.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:06:20 by aadeikal          #+#    #+#             */
/*   Updated: 2025/10/14 14:39:12 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/libft/ft_printf/ft_printf.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

/* #define BUFFER_SIZE 100
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define RESET "\033[0m" */

void test_with_size(const char *test_name, size_t buf_size, const char *format, ...)
{
    char    *buffer_std;
    char    *buffer_ft;
    int     result_std, result_ft;
    va_list args, args_copy;
    
    printf(GREEN "======== Testing: %s (buffer size: %zu) ========\n" RESET, test_name, buf_size);
    printf(BLUE "Format: \"%s\"\n" RESET, format);
    
    // Allocate buffers
    buffer_std = malloc(buf_size + 1); // +1 for potential overflow checking
    buffer_ft = malloc(buf_size + 1);
    if (!buffer_std || !buffer_ft)
    {
        printf(RED "Memory allocation failed\n" RESET);
        if (buffer_std) free(buffer_std);
        if (buffer_ft) free(buffer_ft);
        return;
    }
    
    // Reset buffers
    if (buf_size > 0)
    {
        memset(buffer_std, 'X', buf_size + 1);
        memset(buffer_ft, 'X', buf_size + 1);
    }
    
    // Initialize args
    va_start(args, format);
    va_copy(args_copy, args);
    
    // Run both functions
    result_std = vsnprintf(buffer_std, buf_size, format, args);
    result_ft = ft_snprintf(buffer_ft, buf_size, format, args_copy);
    
    // Clean up
    va_end(args);
    va_end(args_copy);
    
    // Compare results
    printf("Standard return: %d\n", result_std);
    printf("Our return:      %d\n", result_ft);
    
    if (buf_size > 0)
    {
        printf("Standard output: \"");
        for (size_t i = 0; i < buf_size && buffer_std[i] != '\0'; i++)
            printf("%c", buffer_std[i]);
        printf("\"\n");
        
        printf("Our output:      \"");
        for (size_t i = 0; i < buf_size && buffer_ft[i] != '\0'; i++)
            printf("%c", buffer_ft[i]);
        printf("\"\n");
        
        int content_match = (strncmp(buffer_std, buffer_ft, buf_size) == 0);
        
        if (result_std == result_ft && content_match)
            printf(GREEN "✓ Test passed!\n\n" RESET);
        else
            printf(RED "✗ Test failed!\n\n" RESET);
    }
    else
    {
        // For zero-sized buffer, only compare return values
        if (result_std == result_ft)
            printf(GREEN "✓ Test passed (return value only)!\n\n" RESET);
        else
            printf(RED "✗ Test failed (return value)!\n\n" RESET);
    }
    
    free(buffer_std);
    free(buffer_ft);
}

void run_comprehensive_test(const char *test_name, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    
    // Test with different buffer sizes
    va_list args_copy;
    
    // Normal buffer (plenty of space)
    va_copy(args_copy, args);
    vprintf(GREEN "=== %s with normal buffer ===\n" RESET, args_copy);
    va_end(args_copy);
    
    va_copy(args_copy, args);
    test_with_size(test_name, 100, format, va_arg(args_copy, int), va_arg(args_copy, int));
    va_end(args_copy);
    
    // Exact size buffer
    int length;
    va_copy(args_copy, args);
    length = vsnprintf(NULL, 0, format, args_copy) + 1; // +1 for null terminator
    va_end(args_copy);
    
    va_copy(args_copy, args);
    vprintf(GREEN "=== %s with exact buffer size (%d) ===\n" RESET, args_copy);
    va_end(args_copy);
    
    va_copy(args_copy, args);
    test_with_size(test_name, length, format, va_arg(args_copy, int), va_arg(args_copy, int));
    va_end(args_copy);
    
    // Small buffer (truncation)
    if (length > 5)
    {
        va_copy(args_copy, args);
        vprintf(GREEN "=== %s with small buffer (5) ===\n" RESET, args_copy);
        va_end(args_copy);
        
        va_copy(args_copy, args);
        test_with_size(test_name, 5, format, va_arg(args_copy, int), va_arg(args_copy, int));
        va_end(args_copy);
    }
    
    // Zero buffer (just return length)
    va_copy(args_copy, args);
    vprintf(GREEN "=== %s with zero buffer ===\n" RESET, args_copy);
    va_end(args_copy);
    
    va_copy(args_copy, args);
    test_with_size(test_name, 0, format, va_arg(args_copy, int), va_arg(args_copy, int));
    va_end(args_copy);
    
    // Single character buffer (can only fit null terminator)
    va_copy(args_copy, args);
    vprintf(GREEN "=== %s with single-byte buffer ===\n" RESET, args_copy);
    va_end(args_copy);
    
    va_copy(args_copy, args);
    test_with_size(test_name, 1, format, va_arg(args_copy, int), va_arg(args_copy, int));
    va_end(args_copy);
    
    va_end(args);
}

int main(void)
{
    // Test each format specifier with different buffer sizes
    
    // Character tests
    run_comprehensive_test("Character %%c", "Character: %c", "Character test", 'A', 'A');
    
    // String tests
    run_comprehensive_test("String %%s", "String: %s", "String test", "Hello, world!", "Hello, world!");
    run_comprehensive_test("NULL string", "String: %s", "NULL string test", NULL, NULL);
    run_comprehensive_test("Empty string", "String: '%s'", "Empty string test", "", "");
    
    // Integer tests
    run_comprehensive_test("Integer %%d", "Integer: %d", "Integer test", 12345, 12345);
    run_comprehensive_test("Integer MIN", "Integer MIN: %d", "Integer MIN test", INT_MIN, INT_MIN);
    run_comprehensive_test("Integer MAX", "Integer MAX: %d", "Integer MAX test", INT_MAX, INT_MAX);
    run_comprehensive_test("Integer zero", "Integer zero: %d", "Integer zero test", 0, 0);
    run_comprehensive_test("Integer negative", "Integer neg: %d", "Integer negative test", -12345, -12345);
    
    // Unsigned tests
    run_comprehensive_test("Unsigned %%u", "Unsigned: %u", "Unsigned test", 12345u, 12345u);
    run_comprehensive_test("Unsigned MAX", "Unsigned MAX: %u", "Unsigned MAX test", UINT_MAX, UINT_MAX);
    
    // Hex tests
    run_comprehensive_test("Hex lowercase %%x", "Hex: %x", "Hex lowercase test", 0xabcdef, 0xabcdef);
    run_comprehensive_test("Hex uppercase %%X", "Hex: %X", "Hex uppercase test", 0xABCDEF, 0xABCDEF);
    
    // Pointer tests
    int dummy = 42;
    run_comprehensive_test("Pointer %%p", "Pointer: %p", "Pointer test", &dummy, &dummy);
    run_comprehensive_test("NULL pointer", "NULL pointer: %p", "NULL pointer test", NULL, NULL);
    
    // Percent sign test
    run_comprehensive_test("Percent %%%%", "Percent: %%", "Percent test", 0, 0);
    
    // Mixed format test
    run_comprehensive_test("Mixed formats", "Mixed: %d %s %c %x %%", "Mixed formats test", 
                          123, 123, "test", "test", 'Z', 'Z', 0xabc, 0xabc);
    
    return (0);
}