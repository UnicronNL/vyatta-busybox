/* vi: set sw=4 ts=4: */
/*
 * Mini find implementation for busybox
 *
 *
 * Copyright (C) 1999,2000,2001 by Lineo, inc.
 * Written by Erik Andersen <andersen@lineo.com>, <andersee@debian.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include "busybox.h"
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>


static char *pattern = NULL;
static char *directory = ".";
static int dereferenceFlag = FALSE;

static int fileAction(const char *fileName, struct stat *statbuf, void* junk)
{
	if (pattern == NULL)
		puts(fileName);
	else {
		char *tmp = strrchr(fileName, '/');

		if (tmp == NULL)
			tmp = (char *) fileName;
		else
			tmp++;
		if (check_wildcard_match(tmp, pattern) == TRUE)
			puts(fileName);
	}
	return (TRUE);
}

int find_main(int argc, char **argv)
{
	/* peel off the "find" */
	argc--;
	argv++;

	if (argc > 0 && **argv != '-') {
		directory = *argv;
		argc--;
		argv++;
	}

	/* Parse any options */
	while (argc > 0 && **argv == '-') {
		int stopit = FALSE;

		while (*++(*argv) && stopit == FALSE)
			switch (**argv) {
			case 'f':
				if (strcmp(*argv, "follow") == 0) {
					argc--;
					argv++;
					dereferenceFlag = TRUE;
				}
				break;
			case 'n':
				if (strcmp(*argv, "name") == 0) {
					if (argc-- > 1) {
						pattern = *(++argv);
						stopit = TRUE;
					} else {
						usage(find_usage);
					}
				}
				break;
			case '-':
				/* Ignore all long options */
				break;
			default:
				usage(find_usage);
			}
		if (argc-- > 1)
			argv++;
		if (**argv != '-')
			break;
		else
			break;
	}

	if (recursive_action(directory, TRUE, FALSE, FALSE,
						fileAction, fileAction, NULL) == FALSE) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
