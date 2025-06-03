
CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall 
SRCDIR = src
OUTDIR = x64
ARCH = -m64

SOURCES := $(wildcard $(SRCDIR)/*.c)
BINARIES := $(patsubst $(SRCDIR)/%.c,$(OUTDIR)/%.exe,$(SOURCES))

all: $(BINARIES)

$(OUTDIR)/%.exe: $(SRCDIR)/%.c
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(ARCH) -o $@ $< 2>/dev/null

clean:
	rm -rf $(OUTDIR)/*.exe

.PHONY: all clean