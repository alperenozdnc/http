/*
 * miscellaneous helpers that are too big to not be listed as a helper, but too
 * small to deserve its own source file.
 */

#pragma once

/*
 * helper for declaring that the command line interface will be instantiated
 * with no options.
 */
#define EMPTY_OPTS                                                             \
    (opt_s *[]) {                                                              \
        NULL                                                                   \
    }
