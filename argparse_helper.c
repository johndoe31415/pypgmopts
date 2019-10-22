/*
 *   This file supplements auto-generated code of pypgmopts.
 *   https://github.com/johndoe31415/pypgmopts
 */

#include <stdbool.h>
#include <string.h>
#include "argparse_helper.h"

bool argparse_argument_one_of_choices(const char *value, const char **value_list) {
	while (*value_list) {
		if (!strcmp(*value_list, value)) {
			return true;
		}
		value_list++;
	}
	return false;
}
