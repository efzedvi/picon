/**
 * @author      : faraz
 * @file        : utils
 * @created     : Fri 17 Dec 2021 07:27:16 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "picon/utils.h"
#include "picon/printf.h"
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <time.h>

#include "rtos.h"

int strequal(char *s1, char *s2)
{
	if (!s1 || !s2) return(0);

	return(strcasecmp(s1,s2)==0);
}

char* strlower(char *s)
{
	while (*s) {
		*s = tolower((int)*s);
		s++;
	}
	return s;
}

char* strupper(char *s)
{
	while (*s) {
		*s = toupper((int)*s);
		s++;
	}
	return s;
}

int strnum(char *s)
{
	while (*s) {
		if (!isdigit((int)*s)) return 0;
		s++;
	}
	return 1;
}

char* chomp(char *s)
{
	int i=0;
	if (s) {
		for(i=strlen(s)-1; i>=0; i--) {
			if (isspace((int)s[i]))
				s[i] = '\0';
			else
				break;
		}
	}
	return s;
}

int opt_init(PICON_OPT *sopt, const char *opt_string)
{
	if (!sopt || !opt_string) return -1;

	sopt->optind = 0;
	sopt->opterr = 0;
	sopt->optopt = 0;
	sopt->optarg = NULL;
	sopt->opt_string = opt_string;

	return 0;
}

int getopt(PICON_OPT *sopt, int argc, char **argv)
{
	static int optpos = 1;
	const char *arg;

	if (!sopt) return -1;

	/* Reset? */
	if (sopt->optind == 0) {
		sopt->optind = 1;
		optpos = 1;
	}

	if (sopt->optind >= argc) return -1;

	arg = argv[sopt->optind];
	if (arg && strcmp(arg, "--") == 0) {
		sopt->optind++;
		return -1;
	} else if (!arg || arg[0] != '-' || !isalnum((int)arg[1])) {
		return -1;
	} else {
		const char *opt = strchr(sopt->opt_string, arg[optpos]);
		sopt->optopt = arg[optpos];

		if (!opt) return '?';

		if (opt[1] == ':') {
			if (arg[optpos + 1]) {
				sopt->optarg = (char *)arg + optpos + 1;
				sopt->optind++;
				optpos = 1;
				return sopt->optopt;
			} else if (sopt->optind+1<argc && argv[sopt->optind + 1]) {
				sopt->optarg = (char *)argv[sopt->optind + 1];
				sopt->optind += 2;
				optpos = 1;
				return sopt->optopt;
			} else {
				return ':';
			}
		} else {
			if (!arg[++optpos]) {
				sopt->optind++;
				optpos = 1;
			}
			return sopt->optopt;
		}
	}
}


char *ftos(float f, char *buf, int size)
{
	int	d;

	d = (int) (f * 1000);

	if (f > 0) {
		snprintf(buf, size, "%d.%03d", d / 1000, d % 1000);
	} else {
		snprintf(buf, size, "-%d.%03d", d / 1000, d % 1000);
	}

	return buf;
}

float stof(const char* s)
{
	float 	res = 0, fact = 1;
	char	dot = 0;
	short int d = 0;

	if (*s == '-') {
		s++;
		fact = -1.0;
	}

	for (; *s; s++){
		if (*s == '.') {
			dot = 1;
			continue;
		}

		d = *s - '0';
		if (d >= 0 && d <= 9) {
			if (dot) fact /= 10.0;
			res = res * 10.0 + (float)d;
		}
	}
	return res * fact;
}

inline rtos_tick_type_t diff_ticks(rtos_tick_type_t t0, rtos_tick_type_t t1)
{
	if (t1 > t0)
		return (t1 - t0);

	return ~(rtos_tick_type_t)0 - t0 + 1 + t1;
}


