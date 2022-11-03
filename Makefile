# SPDX-License-Identifier: GPL-2.0-or-later
flags = -g -O2 -Wall -Werror -I src/
head  = src/textsearch.h
obj   = src/textsearch.o src/kmp.o src/bm.o src/sunday.o
demo  = examples/simple

all: $(demo)

%.o:%.c $(head)
	@ echo -e "  \e[32mCC\e[0m	" $@
	@ gcc -o $@ -c $< $(flags)

$(demo): $(obj) $(addsuffix .c,$(demo))
	@ echo -e "  \e[34mMKELF\e[0m	" $@
	@ gcc -o $@ $@.c $(obj) $(flags)

clean:
	@ rm -f $(obj) $(demo)
