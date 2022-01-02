/**
 * @author      : faraz 
 * @file        : utils.h
 * @created     : Fri 17 Dec 2021 07:20:48 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef UTILS_H
#define UTILS_H

#include "rtos.h"

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#define BIT(n)          (1u<<(n))
#define SET(a,b)        (a |=  BIT(b))
#define CLR(a,b)        (a &= ~BIT(b))
#define TOGGLE(a,b)     (a ^= ~BIT(b))
#define GETBIT(a,b)     ( ( (a) & BIT(b) ) ? 1 : 0)
#define SETBIT(a,b,x)   ( (x) ? SET(a,b) : CLR(a,b) )
#define BIT_RANGE(a,b)   ( ((1<<(b+1)) - 1) - ((1<<(a)) -1 ) )

#ifndef MIN
#define MIN(a,b)	(((a) < (b))? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b)	(((a) > (b))? (a) : (b))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))
#endif

#define CONTROL(c) ((c) & 0x1F)

int     strequal(char *s1, char *s2);
char*	strlower(char *s);
char*	strupper(char *s);
int     strnum(char *s);
char    *chomp(char *s);

char *ftos(float f, char *buf, int size);
float stof(const char* s);

// For command line argument parsing
typedef struct _opt {
	int optind;
	int opterr;
	int optopt;
	char *optarg;
	const char *opt_string;
} PICON_OPT;

int opt_init(PICON_OPT *sopt, const char *optstring);
int getopt(PICON_OPT *sopt, int argc, char **argv);

rtos_tick_type_t diff_ticks(rtos_tick_type_t t0, rtos_tick_type_t t1);

#endif /* end of include guard UTILS_H */

