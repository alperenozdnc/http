#pragma once

#include <string.h>

static char *KNOWN_EXTENSIONS[] = {
    "html", "htm",  "css", "js",  "mjs",  "png",  "jpg",   "jpeg",
    "gif",  "webp", "svg", "ico", "json", "xml",  "csv",   "txt",
    "mp3",  "wav",  "ogg", "mp4", "webm", "woff", "woff2", "ttf",
    "otf",  "pdf",  "zip", "tar", "gz",   "wasm", NULL};

static char *KNOWN_CONTENT_TYPES[] = {"text/html; charset=utf-8",
                                      "text/html; charset=utf-8",
                                      "text/css; charset=utf-8",
                                      "application/javascript",
                                      "application/javascript",
                                      "image/png",
                                      "image/jpeg",
                                      "image/jpeg",
                                      "image/gif",
                                      "image/webp",
                                      "image/svg+xml",
                                      "image/x-icon",
                                      "application/json",
                                      "application/xml",
                                      "text/csv",
                                      "text/plain; charset=utf-8",
                                      "audio/mpeg",
                                      "audio/wav",
                                      "audio/ogg",
                                      "video/mp4",
                                      "video/webm",
                                      "font/woff",
                                      "font/woff2",
                                      "font/ttf",
                                      "font/otf",
                                      "application/pdf",
                                      "application/zip",
                                      "application/x-tar",
                                      "application/gzip",
                                      "application/wasm",
                                      NULL};

static char *FALLBACK_CONTENT_TYPE = "application/octet-stream";
