/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include "textsearch.h"

#define TEST_STRING "abcceabcaabcd"
#define TEST_PATTERN "abcd"
#define TEST_OFFSET 9

int main()
{
    struct ts_context *context;
    struct ts_linear linear;

    printf("test bm algorithm: ");
    context = textsearch_prepare("bm", TEST_PATTERN, sizeof(TEST_PATTERN) - 1, 0);
    if (!context || textsearch_linear_find(context, &linear, TEST_STRING, sizeof(TEST_STRING) - 1) != TEST_OFFSET) {
        printf("failed\n");
        return -1;
    }
    printf("passed\n");
    textsearch_destroy(context);

    printf("test kmp algorithm: ");
    context = textsearch_prepare("kmp", TEST_PATTERN, sizeof(TEST_PATTERN) - 1, 0);
    if (!context || textsearch_linear_find(context, &linear, TEST_STRING, sizeof(TEST_STRING) - 1) != TEST_OFFSET) {
        printf("failed\n");
        return -1;
    }
    printf("passed\n");
    textsearch_destroy(context);

    printf("test sunday algorithm: ");
    context = textsearch_prepare("sunday", TEST_PATTERN, sizeof(TEST_PATTERN) - 1, 0);
    if (!context || textsearch_linear_find(context, &linear, TEST_STRING, sizeof(TEST_STRING) - 1) != TEST_OFFSET) {
        printf("failed\n");
        return -1;
    }
    printf("passed\n");
    textsearch_destroy(context);

    return 0;
}
