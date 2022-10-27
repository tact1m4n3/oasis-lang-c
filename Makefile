BIN := bin
OASISC := $(BIN)/oasisc

OASISC_FILES := $(wildcard src/*.c)

all: $(OASISC)

$(OASISC): $(OASISC_FILES)
	mkdir -p $(dir $@)
	gcc -o $@ $^

run: $(OASISC)
	$(OASISC) examples/test.oasis test.s
	gcc -o test test.s src/lib/object.c
	./test

dis:
	gcc -g -c examples/test.c -o test.o
	objdump -drwC test.o

clean:
	rm -rf $(BIN)
