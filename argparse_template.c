/*
 *   This file was AUTO-GENERATED by pypgmopts.
 *
 *   https://github.com/johndoe31415/pypgmopts
 *
 *   Do not edit it by hand, your changes will be overwritten.
 *
 *   Generated at: ${now.strftime("%Y-%m-%d %H:%M:%S")}
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdarg.h>
#include <string.h>
#include "argparse.h"

static enum argparse_option_t last_parsed_option;
static char last_error_message[256];
static const char *option_texts[] = {
%for opt in opts:
	[ARG_${opt.name.upper()}] = "${opt.opt_text}",
%endfor
};

enum argparse_option_internal_t {
%for opt in [ opt for opt in opts if opt.opt_short is not None ]:
	ARG_${opt.name.upper()}_SHORT = '${opt.opt_short}',
%endfor
%for (oid, opt) in enumerate([ opt for opt in opts if opt.opt_long is not None ], 1000):
	ARG_${opt.name.upper()}_LONG = ${oid},
%endfor
};

static void errmsg_callback(const char *errmsg, ...) {
	va_list ap;
	va_start(ap, errmsg);
	vsnprintf(last_error_message, sizeof(last_error_message), errmsg, ap);
	va_end(ap);
}

static void errmsg_option_callback(enum argparse_option_t error_option, const char *errmsg, ...) {
	last_parsed_option = error_option;

	va_list ap;
	va_start(ap, errmsg);
	vsnprintf(last_error_message, sizeof(last_error_message), errmsg, ap);
	va_end(ap);
}

bool argparse_parse(int argc, char **argv, argparse_callback_t argument_callback, argparse_plausibilization_callback_t plausibilization_callback) {
	last_parsed_option = ARGPARSE_NO_OPTION;
	const char *short_options = "${short_opts_string}";
	struct option long_options[] = {
	%for opt in [ opt for opt in opts if opt.opt_long is not None ]:
		{ ${"%-35s" % ("\"" + opt.opt_long + "\",")} ${"required_argument" if opt.requires_parameter else "no_argument"}, 0, ARG_${opt.name.upper()}_LONG },
	%endfor
		{ 0 }
	};

	while (true) {
		int optval = getopt_long(argc, argv, short_options, long_options, NULL);
		if (optval == -1) {
			break;
		}
		last_error_message[0] = 0;
		enum argparse_option_internal_t arg = (enum argparse_option_internal_t)optval;
		switch (arg) {
%for opt in [ opt for opt in opts if not opt.positional ]:
%if opt.opt_short is not None:
			case ARG_${opt.name.upper()}_SHORT:
%endif
%if opt.opt_long is not None:
			case ARG_${opt.name.upper()}_LONG:
%endif
				last_parsed_option = ARG_${opt.name.upper()};
				if (!argument_callback(ARG_${opt.name.upper()}, optarg, errmsg_callback)) {
					return false;
				}
				break;

%endfor
			default:
				last_parsed_option = ARGPARSE_NO_OPTION;
				errmsg_callback("unrecognized option supplied");
				return false;
		}
	}

	const int positional_argument_cnt = argc - optind;
%if have_flexible_positional_argument:
	const int flexible_positional_args_cnt = positional_argument_cnt - ${sum(1 for opt in opts if (opt.positional and (opt.nargs is None)))};
%endif
%if min_pos is not None:
	if (positional_argument_cnt < ${min_pos}) {
		errmsg_callback("expected a minimum of ${min_pos} positional arguments, %d given.", positional_argument_cnt);
		return false;
	}
%endif
%if max_pos is not None:
	if (positional_argument_cnt > ${max_pos}) {
		errmsg_callback("expected a maximum of ${max_pos} positional arguments, %d given.", positional_argument_cnt);
		return false;
	}
%endif

%if any(opt.positional for opt in opts):
	int positional_index = optind;
%for opt in [ opt for opt in opts if opt.positional ]:
	last_parsed_option = ARG_${opt.name.upper()};
	%if opt.nargs is None:
	if (!argument_callback(ARG_${opt.name.upper()}, argv[positional_index++], errmsg_callback)) {
		return false;
	}
	%else:
	for (int i = 0; i < flexible_positional_args_cnt; i++) {
		if (!argument_callback(ARG_${opt.name.upper()}, argv[positional_index++], errmsg_callback)) {
			return false;
		}
	}
	%endif
%endfor

%endif
	if (plausibilization_callback) {
		if (!plausibilization_callback(errmsg_option_callback)) {
			return false;
		}
	}
	return true;
}

void argparse_show_syntax(void) {
%for line in help_text:
	fprintf(stderr, "${line.replace('"', '\\"')}\n");
%endfor
}

void argparse_parse_or_quit(int argc, char **argv, argparse_callback_t argument_callback, argparse_plausibilization_callback_t plausibilization_callback) {
	if (!argparse_parse(argc, argv, argument_callback, plausibilization_callback)) {
		if (last_parsed_option != ARGPARSE_NO_OPTION) {
			if (last_error_message[0]) {
				fprintf(stderr, "%s: error parsing argument %s -- %s\n", argv[0], option_texts[last_parsed_option], last_error_message);
			} else {
				fprintf(stderr, "%s: error parsing argument %s -- no details available\n", argv[0], option_texts[last_parsed_option]);
			}
		}
		argparse_show_syntax();
		exit(EXIT_FAILURE);
	}
}

bool argparse_argument_one_of_choices(const char *value, const char **value_list) {
	while (*value_list) {
		if (!strcmp(*value_list, value)) {
			return true;
		}
		value_list++;
	}
	errmsg_callback("'%s' is not a valid choice for this option", value);
	return false;
}

#ifdef __ARGPARSE_MAIN__
/*   gcc -D __ARGPARSE_MAIN__ -O2 -Wall -o argparse argparse.c
*/

static const char *option_enum_to_str(enum argparse_option_t option) {
	switch (option) {
%for opt in opts:
		case ARG_${opt.name.upper()}: return "ARG_${opt.name.upper()}";
%endfor
	}
	return "UNKNOWN";
}

bool arg_print_callback(enum argparse_option_t option, const char *value, argparse_errmsg_callback_t errmsg_callback) {
	fprintf(stderr, "%s = \"%s\"\n", option_enum_to_str(option), value);
	return true;
}

int main(int argc, char **argv) {
	argparse_parse_or_quit(argc, argv, arg_print_callback, NULL);
	return 0;
}
#endif
