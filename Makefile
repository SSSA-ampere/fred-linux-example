BIN = fred-test-cli
LIBFRED = fred
LIBFRED_DIR = fred_support

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

CFLAGS += -std=gnu99 -g -Wall -Werror -fpic
LDFLAGS += -lpthread

$(BIN): $(OBJS) $(LIBFRED)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) -L $(LIBFRED_DIR) -l$(LIBFRED)
	
$(LIBFRED):
	$(MAKE) -C $(LIBFRED_DIR)

# include all dep makefiles generated using the next rule
-include $(DEPS)

# Pattern rule for generating makefiles rules based
# on headers includes dependencies using the C preprocessor
%.d: %.c
	$(CPP) $< -MM -MT $(@:.d=.o) > $@

.PHONY: clean
clean:
	rm -f $(BIN) $(OBJS) $(DEPS)
	$(MAKE) $@ -C $(LIBFRED_DIR)

