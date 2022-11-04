/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include "textsearch.h"
#include  <stdio.h>
#include  <sys/stat.h>
#include  <string.h>
#include  <assert.h>
#include  <stdlib.h>

#define TEST_STRING "abcceabcaabcd"
#define TEST_PATTERN "abcd"
#define TEST_OFFSET 9


struct array {
    void *elts;
    int  nelts;
    size_t size;
    int  nalloc;
};

struct array *array_create(int nalloc, size_t size) {
    struct array *array;
    array = malloc(sizeof(struct array));
    
    array->elts = malloc(size * nalloc);
    array->nelts = 0;
    array->size = size;
    array->nalloc = nalloc;

    return array;
}

void *array_push(struct array *a) {
    void        *elt, *new = NULL;
    size_t       size = 0;
    if (a->nelts == a->nalloc) {
        assert(size);
        assert(new);
        printf("fuck you\n");
        exit(-1);
    }
    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts++;
    return elt;
}

struct str {
    char *data;
    int  len;
};

struct pattern {
    struct str str;
    void *ctx;
};

struct dict {
    struct array *patterns;
};

struct request {
    struct array *strs;
};

#define PATTERN_MAX_SIZE 1024
struct dict *read_dict(char *file) {
    FILE *fp = NULL;
    fp = fopen(file, "r");
    struct dict *dict = (struct dict *)malloc(sizeof(struct dict));
    dict->patterns = array_create(10240, sizeof(struct pattern));

    assert(dict);

    assert(fp);

    for (;;) {
        char buf[PATTERN_MAX_SIZE];
        memset(buf, 0, sizeof(buf));
        if (fgets(buf, sizeof(buf), fp) == NULL) {
            if (feof(fp)) {
                break;
            }else {
                fclose(fp);
                return NULL;
            }
        }
        struct pattern *pattern = array_push(dict->patterns);
        pattern->ctx =  textsearch_prepare("kmp", buf, strlen(buf), 0);

        pattern->str.data  =  strdup(buf);
        assert(pattern->str.data);
        // memcpy(pattern->str, buf, strlen(buf) + 1);
        pattern->str.len = strlen(buf);
    }
    return dict;
}

#define PATTERN_MAX_SIZE 1024
struct request *read_requests(char *file) {
    FILE *fp = NULL;
    fp = fopen(file, "r");
    assert(fp);

    struct request *request = (struct request *)malloc(sizeof(struct request));
    request->strs = array_create(10240, sizeof(struct str));

    for (;;) {
        char buf[PATTERN_MAX_SIZE];
        memset(buf, 0, sizeof(buf));
        if (fgets(buf, sizeof(buf), fp) == NULL) {
            if (feof(fp)) {
                break;
            }else {
                fclose(fp);
                return NULL;
            }
        }
        struct str *master = array_push(request->strs);
        master->data =  strdup(buf);
        assert(master->data);
        // memcpy(str->str, buf, strlen(buf) + 1);
        master->len = strlen(buf);
    }
    return request;
}


int main()
{

    struct dict * dict = read_dict("/home/fashion/project/nodeproject/node_server/pattern.txt");
    struct request *request = read_requests("/home/fashion/project/nodeproject/node_server/master.txt");

    assert(dict);
    assert(request);


    int nrequest, i, npattern, j;

    nrequest = request->strs->nelts;
    struct str *hys = request->strs->elts;
    
    npattern =  dict->patterns->nelts;
    struct pattern *nds = dict->patterns->elts;

    for (i = 0; i < nrequest; i++) {
        struct str hy = hys[i];
        for (j = 0; j < npattern; j++) {
            struct pattern nd = nds[j];

            struct ts_linear linear;

            if (textsearch_linear_find(nd.ctx, &linear, hy.data, hy.len) != 0) {
                // printf("hy is %s, nd is %s\n", hy.data, nd.str.data);
            }
            // char *matched = strstr(hy.data, nd.str.data);
            // if (matched != NULL) {
            //     // printf("hy is %s, nd is %s\n", hy.data, nd.str.data);
            // }
        }
    }

    // struct ts_context *context;
    // struct ts_linear linear;

    // printf("test bm algorithm: ");
    // context = textsearch_prepare("bm", TEST_PATTERN, sizeof(TEST_PATTERN) - 1, 0);
    // if (!context || textsearch_linear_find(context, &linear, TEST_STRING, sizeof(TEST_STRING) - 1) != TEST_OFFSET) {
    //     printf("failed\n");
    //     return -1;
    // }
    // printf("passed\n");
    // textsearch_destroy(context);

    

    // printf("test kmp algorithm: ");
    // context = textsearch_prepare("kmp", TEST_PATTERN, sizeof(TEST_PATTERN) - 1, 0);
    // if (!context || textsearch_linear_find(context, &linear, TEST_STRING, sizeof(TEST_STRING) - 1) != TEST_OFFSET) {
    //     printf("failed\n");
    //     return -1;
    // }
    // printf("passed\n");
    // textsearch_destroy(context);




    // printf("test sunday algorithm: ");
    // context = textsearch_prepare("sunday", TEST_PATTERN, sizeof(TEST_PATTERN) - 1, 0);
    // if (!context || textsearch_linear_find(context, &linear, TEST_STRING, sizeof(TEST_STRING) - 1) != TEST_OFFSET) {
    //     printf("failed\n");
    //     return -1;
    // }
    // printf("passed\n");
    // textsearch_destroy(context);

    return 0;
}
