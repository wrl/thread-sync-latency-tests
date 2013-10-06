CC := clang
LD := $(cc)

OUTDIR := build
SRCDIR := src

CFLAGS  += -Wall -Werror -std=c99 -D_GNU_SOURCE -pthread -I.
LDFLAGS += -pthread

TARGETS  = $(OUTDIR)/control
TARGETS += $(OUTDIR)/pthread_condvar

.PHONY: all test clean

all: $(TARGETS)

test:
	mkdir -p results
	$(OUTDIR)/control > results/control
	$(OUTDIR)/pthread_condvar > results/pthread_condvar
	@echo ''
	ruby analyze.rb results/*

clean:
	rm -rf $(OUTDIR)

$(OUTDIR):
	mkdir -p $@

$(OUTDIR)/%.o: $(SRCDIR)/%.c | $(OUTDIR)
	$(CC) -c $(CFLAGS) -o $@ $^

control: $(OUTDIR)/control.o | $(OUTDIR)
	$(LD) $(CFLAGS) $(LDFLAGS) -o $@ $^

pthread_condvar: $(OUTDIR)/pthread_condvar.o | $(OUTDIR)
	$(LD) $(CFLAGS) $(LDFLAGS) -o $@ $^
