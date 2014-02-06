
PREFIX ?= /usr/local
DESTDIR ?= sly

OS = $(shell uname)
CC ?= cc
AR = ar
LN = ln
RM = rm
VALGRIND ?= valgrind
STRIP = strip

LIB_NAME = sly
VERSION_MAJOR = 0
VERSION_MINOR = 1

DEPS = $(shell test -d deps/ && ls deps/)

TARGET_NAME = lib$(LIB_NAME)
TARGET_STATIC = $(TARGET_NAME).a
TARGET_DSOLIB = $(TARGET_NAME).so.$(VERSION_MAJOR).$(VERSION_MINOR)
TARGET_DYLIB = $(TARGET_NAME).$(VERSION_MAJOR).$(VERSION_MINOR).dylib
TARGET_DSO = $(TARGET_NAME).so

CFLAGS += -Iinclude -Ideps \
          -std=c99 -O2 \
          -fvisibility=hidden \
          -fPIC -Wno-switch

LDFLAGS += -shared \
           -soname $(TARGET_DSO).$(VERSION_MAJOR)

OSX_LDFLAGS += -lc \
               -Wl,-install_name,$(TARGET_DSO) \
               -o $(TARGET_DSOLIB) \

SRC = $(wildcard src/*.c)
OBJS = $(SRC:.c=.o)

## deps
SRC += $(wildcard deps/trim/*.c)
SRC += $(wildcard deps/list/*.c)
SRC += $(wildcard deps/buffer/*.c)
SRC += $(wildcard deps/hash/*.c)

TEST_SRC = $(wildcard test/*.c)
TEST_OBJS = $(TEST_SRC:.c=.o)
TEST_MAIN = sly-test

## test deps
TEST_DEPS = deps/ok/libok.a

ifneq ("Darwin","$(OS)")
	CFLAGS += -lm
endif

ifdef DEBUG
	CFLAGS += -DSLY_DEBUG
endif

all: deps $(TARGET_STATIC) $(TARGET_DSO)

$(TARGET_STATIC): $(OBJS)
	@echo "  LIBTOOL-STATIC $(TARGET_STATIC)"
	@$(AR) crus $(TARGET_STATIC) $(OBJS)

$(TARGET_DSO): $(OBJS)
	@echo "  LIBTOOL-SHARED $(TARGET_DSOLIB)"
	@echo "  LIBTOOL-SHARED $(TARGET_DSO)"
	@echo "  LIBTOOL-SHARED $(TARGET_DSO).$(VERSION_MAJOR)"
ifeq ("Darwin","$(OS)")
	@$(CC) -shared $(OBJS) $(OSX_LDFLAGS) -o $(TARGET_DSOLIB)
	@$(LN) -s $(TARGET_DSOLIB) $(TARGET_DSO)
	@$(LN) -s $(TARGET_DSOLIB) $(TARGET_DSO).$(VERSION_MAJOR)
else
	@$(CC) -shared $(OBJS) -o $(TARGET_DSOLIB)
	@$(LN) -s $(TARGET_DSOLIB) $(TARGET_DSO)
	@$(LN) -s $(TARGET_DSOLIB) $(TARGET_DSO).$(VERSION_MAJOR)
	@$(STRIP) --strip-unneeded $(TARGET_DSO)
endif

$(OBJS):
	@echo "  CC(target) $@"
	@$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)

$(TEST_OBJS):
	@echo "  CC(target) $@"
	@$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)

deps: $(DEPS)

$(DEPS):
	@if test -f deps/$@/Makefile; then \
		echo "  MAKE(target) deps/$@"; \
		$(MAKE) -C deps/$@ >/dev/null 2>&1; \
		fi

check: test
	$(VALGRIND) --leak-check=full ./$(TEST_MAIN)

test: $(TEST_OBJS)
	@#echo "  LINK(target) ($(TEST_MAIN))"
	@$(CC) $(TEST_DEPS) $(TEST_OBJS) test.c \
		./$(TARGET_STATIC) \
		$(CFLAGS) -o $(TEST_MAIN)
	@#echo "  RUN $(TEST_MAIN)"
	@echo
	@./$(TEST_MAIN)

clean:
	@for dep in $(DEPS); do \
		if test -f deps/$$dep/Makefile; then \
			echo "  MAKE(clean) deps/$$dep"; \
			$(MAKE) clean -C deps/$$dep >/dev/null 2>&1; \
		fi \
	done
	@for item in \
		$(TEST_MAIN) $(OBJS) $(TEST_OBJS) $(TARGET_STATIC) \
		$(TARGET_DSOLIB) $(TARGET_DSO).$(VERSION_MAJOR) \
		$(TARGET_DSO) $(TARGET_DYLIB) $(TEST_DB_PATH) \
		; do \
		echo "  RM $$item"; \
		$(RM) -rf $$item; \
	done;

install: all
	test -d $(PREFIX)/$(DESTDIR) || mkdir $(PREFIX)/$(DESTDIR)
	install $(LIB_NAME).h $(PREFIX)/include
	cp include/$(LIB_NAME)/*.h $(PREFIX)/include/$(DESTDIR)
	install $(TARGET_STATIC) $(PREFIX)/lib
	install $(TARGET_DSO) $(PREFIX)/lib

uninstall:
	rm -rf $(PREFIX)/$(DESTDIR)
	rm -f $(PREFIX)/lib/$(TARGET_STATIC)
	rm -f $(PREFIX)/lib/$(TARGET_DSO)

.PHONY: test deps clean
