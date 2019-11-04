


OBJECTS = out/Random.o

OUTPUT = librandom.a

CDIALECT = c11

INCLUDES = ./include

ALL_CFLAGS = $(CFLAGS) $(foreach include,$(INCLUDES),-I$(include)) -fPIC

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(AR) rcs $(OUTPUT) $(OBJECTS)

test: $(OUTPUT)
	+$(MAKE) -C tests

out/:
	mkdir out/	

out/%.o: src/%.c | out/
	$(CC) $(ALL_CFLAGS) -c -o $@ $^



	 
clean:
	rm -rf out/ $(OUTPUT)
	+$(MAKE) -C tests clean
