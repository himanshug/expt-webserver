CC = cc 

CFLAGS=-g -O2 -Wall -Wextra -Ilib -rdynamic -DNDEBUG $(OPTFLAGS)
LIBS=-ldl $(OPTLIBS)
PREFIX?=/usr/local

LIB_SOURCES=$(wildcard lib/*.c)
OBJECTS=$(patsubst %.c,%.o,$(LIB_SOURCES))

BIN_SOURCES=$(wildcard sockets/*.c)
BINS=$(patsubst %.c,%,$(BIN_SOURCES))

TARGET=build/libtlpi.a

# The Target Build
all: clean $(TARGET) bin

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

build:
	@mkdir -p build

bin: CFLAGS += $(TARGET)
bin:
	for i in $(BINS); do cc $$i.c -o $$i $(CFLAGS); done

# The Cleaner
clean:
	rm -rf build $(OBJECTS) $(BINS)

#dev target only changes CFLAGS
dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all


