
CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall 
SRCDIR = src
OUTDIR = x64
ARCH = -m64

FSOURCES := $(wildcard $(SRCDIR)/*_F.c)
OSOURCES := $(wildcard $(SRCDIR)/*_O.c)
FBINARIES := $(patsubst $(SRCDIR)/%_F.c,$(OUTDIR)/%.exe,$(FSOURCES)) 
OBINARIES := $(patsubst $(SRCDIR)/%_O.c,$(OUTDIR)/%.o,$(OSOURCES))

all:
	@mkdir -p $(OUTDIR)
	$(MAKE) exec
	$(MAKE) obj

exec: $(FBINARIES)

$(OUTDIR)/%.exe: $(SRCDIR)/%_F.c
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(ARCH) -o $@ $< 2>/dev/null


obj: $(OBINARIES)

$(OUTDIR)/%.o: $(SRCDIR)/%_O.c
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(ARCH) -c -o $@ $< 2>/dev/null

clean:
	rm -rf $(OUTDIR)/*.exe
	rm -rf $(OUTDIR)/*.o

.PHONY: all clean