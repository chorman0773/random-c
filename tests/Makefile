

TESTS = 00nextint 01nextintb



CDIALECT = c11

INCLUDES = ../include

ALL_CFLAGS = $(CFLAGS) $(foreach include,$(INCLUDES),-I$(include)) -fPIC


all: $(TESTS)

%: %.c
	gcc $(ALL_CFLAGS) -o $@ $^ -L.. -lrandom -lm
	./$@
	rm -f $@

clean:
	rm -f $(TESTS)
