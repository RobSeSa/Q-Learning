EXECBIN  = run

SOURCES  = $(wildcard *.c)
HEADERS  = $(wildcard *.h)
OBJECTS  = $(SOURCES:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra

LINKER   = clang
LFLAGS   = -Wall -lm

.PHONY: all clean spotless 

all: $(EXECBIN) clean

$(EXECBIN): $(OBJECTS)
	$(LINKER) $? $(LFLAGS) -o $@

$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

spotless: clean
	rm -rf $(EXECBIN)

clean:
	rm -rf $(OBJECTS)
