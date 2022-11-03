/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#include <string.h>
#include "textsearch.h"

static struct ts_algorithm *algorithms[] = {
    &bm_algorithm,
    &kmp_algorithm,
    &sunday_algorithm,
};

static struct ts_algorithm *algorithm_find(const char *name)
{
    unsigned int count;

    for (count = 0; count < (sizeof(algorithms) / sizeof(algorithms[0])); ++count) {
        if (!strcmp(algorithms[count]->name, name))
            return algorithms[count];
    }

    return NULL;
}

struct ts_context *textsearch_prepare(const char *name, const void *pattern,
                                      size_t len, unsigned long flags)
{
    struct ts_algorithm *algo;
    struct ts_context *tsc;

    algo = algorithm_find(name);
    if (!algo)
        return NULL;

    tsc = algo->prepare(pattern, len, flags);
    if (!tsc)
        return NULL;

    tsc->algo = algo;
    return tsc;
}

static unsigned int linear_next(struct ts_context *tsc, struct ts_state *tss,
                                unsigned int consumed, const void **dest)
{
    struct ts_linear *linear = tss->pdata;

	if (likely(consumed < linear->len)) {
		*dest = linear->data + consumed;
		return linear->len - consumed;
	}

    return 0;
}

unsigned int textsearch_linear_find(struct ts_context *tsc, struct ts_linear *linear,
                                    const void *data, unsigned int len)
{
    struct ts_algorithm *algo = tsc->algo;

    linear->data = data;
    linear->len = len;
    linear->tss.offset = 0;
    linear->tss.pdata = linear;
    tsc->next_block = linear_next;

    return algo->find(tsc, &linear->tss);
}

unsigned int textsearch_linear_next(struct ts_context *tsc, struct ts_linear *linear)
{
    struct ts_algorithm *algo = tsc->algo;
    return algo->find(tsc, &linear->tss);
}

