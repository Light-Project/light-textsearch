/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#ifndef _MACRO_H_
#define _MACRO_H_

/**
 * container_of - cast a member of a structure out to the containing structure.
 * @ptr: the pointer to the member.
 * @type: the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 */
#ifndef container_of
# define container_of(ptr, type, member) ({             \
    const typeof(((type *)0)->member) *__mptr = (ptr);  \
    (type *)((char *)__mptr - offsetof(type,member));   \
})
#endif
/**
 * min - return minimum of two values of the same or compatible types
 * @a: first value
 * @b: second value
 */
#define min(a, b) ({                \
    typeof(a) _amin = (a);          \
    typeof(a) _bmin = (b);          \
    (void)(&_amin == &_bmin);       \
    _amin < _bmin ? _amin : _bmin;  \
})

/**
 * max - return maximum of two values of the same or compatible types
 * @a: first value
 * @b: second value
 */
#define max(a, b) ({                \
    typeof(a) _amax = (a);          \
    typeof(a) _bmax = (b);          \
    (void)(&_amax == &_bmax);       \
    _amax > _bmax ? _amax : _bmax;  \
})

#ifndef likely
# define likely(x) __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#endif

#endif  /* _MACRO_H_ */
