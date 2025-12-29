/*
 * configuration fields for the command line interface. these values
 * are fed into the program when the command line interface is being
 * instantiated.
 */

#pragma once

#define CMD_NAME "http"
#define CMD_DESC "a basic http server written in c."
#define CMD_USAGE "[cmd] {args}"
#define CMD_FOOTER                                                             \
    "source code can be found at <https://github.com/alperenozdnc/http.git>\n" \
    "all source code under this project is licensed with GPLv3.\n"
#define CMD_HELP_ALIASES CLI_ALIASES("help", "--help")
