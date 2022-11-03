/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#ifndef _TEXTSEARCH_H_
#define _TEXTSEARCH_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "macro.h"

#define TS_IGNORECASE 0x01

/**
 * struct ts_state - search state.
 * @offset: offset for next match.
 * @offset: search context private data.
 */
struct ts_state {
    unsigned int offset;
    void *pdata;
};

struct ts_linear {
    struct ts_state tss;
    const char *data;
    unsigned int len;
};

/**
 * struct ts_context - search context.
 * @ops: chosen search algorithm.
 * @flags: user flags.
 * @next_block: fetch next block of data.
 */
struct ts_context {
    struct ts_algorithm *algo;
    unsigned long flags;
    unsigned int (*next_block)(struct ts_context *tsc, struct ts_state *tss,
                               unsigned int consumed, const void **dest);
};

/**
 * struct ts_algorithm - search algorithm operations.
 * @name: name of search algorithm.
 * @prepare: initialization function to prepare a search.
 * @destroy: destroy algorithm specific parts of a search configuration.
 * @find: find the next occurrence of the pattern.
 * @pattern_get: return pointer of pattern.
 * @pattern_len: return length of pattern.
 */
struct ts_algorithm {
    const char *name;
    struct ts_context *(*prepare)(const void *pattern, size_t len, unsigned long flags);
    void (*destroy)(struct ts_context *tsc);
    unsigned int (*find)(struct ts_context *tsc, struct ts_state *tss);
    const void *(*pattern_get)(struct ts_context *tsc);
    unsigned int (*pattern_len)(struct ts_context *tsc);
};

extern struct ts_algorithm bm_algorithm;
extern struct ts_algorithm kmp_algorithm;
extern struct ts_algorithm sunday_algorithm;

static inline unsigned int textsearch_find(struct ts_context *tsc, struct ts_state *tss)
{
    struct ts_algorithm *algo = tsc->algo;
    tss->offset = 0;
    return algo->find(tsc, tss);
}

static inline unsigned int textsearch_next(struct ts_context *tsc, struct ts_state *tss)
{
    struct ts_algorithm *algo = tsc->algo;
    return algo->find(tsc, tss);
}

static inline const void *textsearch_pattern_get(struct ts_context *tsc)
{
    struct ts_algorithm *algo = tsc->algo;
    return algo->pattern_get(tsc);
}

static inline unsigned int textsearch_pattern_len(struct ts_context *tsc)
{
    struct ts_algorithm *algo = tsc->algo;
    return algo->pattern_len(tsc);
}

static inline void textsearch_destroy(struct ts_context *tsc)
{
    struct ts_algorithm *algo = tsc->algo;
    algo->destroy(tsc);
}

extern struct ts_context *textsearch_prepare(const char *name, const void *pattern, size_t len, unsigned long flags);
extern unsigned int textsearch_linear_find(struct ts_context *tsc, struct ts_linear *linear, const void *data, unsigned int len);
extern unsigned int textsearch_linear_next(struct ts_context *tsc, struct ts_linear *linear);

#endif  /* _TEXTSEARCH_H_ */
