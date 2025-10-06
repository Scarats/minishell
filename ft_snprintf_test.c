/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf_test.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:06:20 by aadeikal          #+#    #+#             */
/*   Updated: 2025/10/06 15:06:29 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/libft/ft_printf/ft_printf.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 100

// Test helper function
void	run_test(const char *test_name, const char *format, ...)
{
	char	buffer1[BUFFER_SIZE];
	char	buffer2[BUFFER_SIZE];

	int result1, result2;
	va_list args1, args2;
	printf(GREEN "======== Testing: %s ========\n" RESET, test_name);
	printf(BLUE "Format: \"%s\"\n" RESET, format);
	// Reset buffers
	memset(buffer1, 'X', BUFFER_SIZE);
	memset(buffer2, 'X', BUFFER_SIZE);
	buffer1[BUFFER_SIZE - 1] = '\0';
	buffer2[BUFFER_SIZE - 1] = '\0';
	// Run standard snprintf
	va_start(args1, format);
	result1 = vsnprintf(buffer1, BUFFER_SIZE, format, args1);
	va_end(args1);
	// Run our ft_snprintf
	va_start(args2, format);
	result2 = ft_snprintf(buffer2, BUFFER_SIZE, format, args2);
	va_end(args2);
	// Compare results
	printf("Standard return: %d\n", result1);
	printf("Our return:      %d\n", result2);
	printf("Standard output: \"%s\"\n", buffer1);
	printf("Our output:      \"%s\"\n", buffer2);
	if (result1 == result2 && strcmp(buffer1, buffer2) == 0)
		printf(GREEN "✓ Test passed!\n\n" RESET);
	else
		printf(RED "✗ Test failed!\n\n" RESET);
}

int	main(void)
{
	char *str = "Hello";
	int num = 42;
	void *ptr = &num;

	printf(PURPLE "===================================\n");
	printf("   FT_SNPRINTF COMPREHENSIVE TESTS\n");
	printf("===================================\n\n" RESET);

	// Test characters
	run_test("Character test - basic", "Character: %c", 'A');
	run_test("Character test - null byte", "Null byte: %c", '\0');

	// Test strings
	run_test("String test - basic", "String: %s", "Hello, world!");
	run_test("String test - empty", "Empty string: %s", "");
	run_test("String test - NULL", "NULL string: %s", NULL);

	// Test integers
	run_test("Integer test - positive", "Positive: %d", 12345);
	run_test("Integer test - negative", "Negative: %i", -9876);
	run_test("Integer test - zero", "Zero: %i", 0);
	run_test("Integer test - INT_MIN", "INT_MIN: %i", -2147483648);
	run_test("Integer test - INT_MAX", "INT_MAX: %i", 2147483647);

	// Test unsigned integers
	run_test("Unsigned test - basic", "Unsigned: %u", 54321);
	run_test("Unsigned test - large", "Large unsigned: %u", 4294967295U);

	// Test hexadecimal
	run_test("Hex test - lowercase", "Hex lowercase: %x", 0xabcdef);
	run_test("Hex test - uppercase", "Hex uppercase: %X", 0xABCDEF);
	run_test("Hex test - zero", "Hex zero: %x", 0);

	// Test pointers
	run_test("Pointer test - valid", "Pointer: %p", ptr);
	run_test("Pointer test - NULL", "NULL pointer: %p", NULL);

	// Test percent sign
	run_test("Percent test", "Percent sign: %%");

	// Test combined formats with colors
	run_test("Combined test",
		"This is a " RED "%s" RESET " with " GREEN "%d" RESET " and " BLUE "%c" RESET "!",
		"test", 42, 'X');

	// Edge cases
	printf(ORANGE "======== Testing Edge Cases ========\n" RESET);

	// Test with small buffer
	char tiny_buf[5];
	int ret = ft_snprintf(tiny_buf, 5, "This is too long for the buffer");
	printf("Small buffer test - return: %d, content: \"%s\"\n", ret, tiny_buf);

	// Test with size 1 (only room for null terminator)
	char null_buf[1];
	ret = ft_snprintf(null_buf, 1, "No room");
	printf("Size 1 buffer test - return: %d\n", ret);

	// Test with NULL buffer (should calculate length only)
	ret = ft_snprintf(NULL, 0, "Calculate length only: %s", "test string");
	printf("NULL buffer test - return: %d\n", ret);

	printf(PURPLE "\n===================================\n");
	printf("          TESTS COMPLETED\n");
	printf("===================================\n" RESET);

	return (0);
}