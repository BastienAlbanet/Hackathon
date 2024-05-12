
BUILD_DIR = build
SRC_DIR = srcs
INC_DIR = includes includes/libz

# Find all the C files we want to compile
COMMON_SRCS := $(shell find $(SRC_DIR)/common -name '*.c')
SERVER_SRCS := $(shell find $(SRC_DIR)/server -name '*.c')
CLIENT_SRCS := $(shell find $(SRC_DIR)/client -name '*.c')

# Prepends BUILD_DIR and replace .c by .o to every src file
COMMON_OBJS := $(COMMON_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
SERVER_OBJS := $(SERVER_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
CLIENT_OBJS := $(CLIENT_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)


SERVER_OBJS := $(SERVER_OBJS) $(COMMON_OBJS)
CLIENT_OBJS := $(CLIENT_OBJS) $(COMMON_OBJS)


INC_FLAGS := $(addprefix -I,$(INC_DIR))
CFLAGS=-Wall -Werror -Wextra
LDFLAGS=-Llibs -lz
#-lc?

all: server client

server: $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o $@ $(LDFLAGS) 

client: $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o $@ $(LDFLAGS)
	
	
# Build step for C source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(INC_FLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm server client || true
	rm -r $(BUILD_DIR) || true

re: clean all

.PHONY: all clean re
