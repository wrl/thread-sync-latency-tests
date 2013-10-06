CC := clang
LD := $(cc)

OUTDIR := build
SRCDIR := src

CFLAGS  += -Wall -Werror -std=c99 -D_GNU_SOURCE -pthread -I.
LDFLAGS += -pthread -lrt

TARGETS  = $(OUTDIR)/control
TARGETS += $(OUTDIR)/pthread_condvar
TARGETS += $(OUTDIR)/posix_sem

.PHONY: all test clean

all: $(TARGETS)

test: all
	mkdir -p results
	$(OUTDIR)/control > results/control
	$(OUTDIR)/pthread_condvar > results/pthread_condvar
	$(OUTDIR)/posix_sem > results/posix_sem
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
