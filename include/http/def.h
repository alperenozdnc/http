/*
 * miscellaneous definitions used throughout the program.
 */

#pragma once

#define HTTP_PREFIX "[http] "
#define HTTP_REQUEST_PREFIX "[http/req] "
#define HTTP_RESPONSE_PREFIX "[http/res] "
#define HTTP_MAX_REQUEST_SIZE 2048

#define OK_STATUS "HTTP/1.0 200 OK"
#define NOT_FOUND_STATUS "HTTP/1.0 404 NOT FOUND"

#define HTML_EXTENSION ".html"

#define NOT_FOUND_PAGE "404" HTML_EXTENSION
#define NOT_FOUND_DEFAULT_PAGE "/.local/http/404.html"

#define CONTENT_TYPE_PREFIX "Content-Type: "
#define CONTENT_LENGTH_PREFIX "Content-Length: "

#define CRLF "\r\n"
#define CRLF_LEN 2 // strlen("\r\n")

#define INDEX_PAGE "index" HTML_EXTENSION
