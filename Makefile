CC = gcc
CFLAGS_BASE = -Wall -Wextra -Werror -Iinclude

BUILD ?= release

ifeq ($(BUILD), release)
	CFLAGS := $(CFLAGS_BASE) -O3
else
	CFLAGS := $(CFLAGS_BASE) -g -fsanitize=address
endif

CLI_DIR = cli
SERVER_DIR = server

BUILD_DIR = build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj

CLI_BUILD_DIR := $(BUILD_DIR)/$(CLI_DIR)
CLI_SRC := $(shell find $(CLI_DIR) -name '*.c')
CLI_OBJ := $(CLI_SRC:$(CLI_DIR)/%.c=$(CLI_BUILD_DIR)/%.o)

$(CLI_BUILD_DIR)/%.o: $(CLI_DIR)/%.c
	@mkdir -p $(dir $@)

	$(CC) $(CFLAGS) -c $< -o $@

SERVER_BUILD_DIR := $(BUILD_DIR)/$(SERVER_DIR)
SERVER_SRC := $(shell find $(SERVER_DIR) -name '*.c')
SERVER_OBJ := $(SERVER_SRC:$(SERVER_DIR)/%.c=$(SERVER_BUILD_DIR)/%.o)

$(SERVER_BUILD_DIR)/%.o: $(SERVER_DIR)/%.c
	@mkdir -p $(dir $@)

	$(CC) $(CFLAGS) -c $< -o $@

HTTP_BIN := $(BIN_DIR)/http

$(HTTP_BIN): $(SERVER_OBJ) $(CLI_OBJ)
	@mkdir -p $(dir $@)

	$(CC) $(CFLAGS) $^ -lezcli -o $@

all: $(HTTP_BIN)

clean:
	@rm -rf $(BUILD_DIR)
