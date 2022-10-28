ARCH := x86_64

BIN := bin
OASISC := $(BIN)/oasisc

OASISC_DEFS := -D OASIS_X64
OASISC_FILES := $(wildcard src/*.c)

all: $(OASISC)

$(OASISC): $(OASISC_FILES)
	mkdir -p $(dir $@)
	gcc $(OASISC_DEFS) -o $@ $^

run: $(OASISC)
	$(OASISC) examples/test.oasis test.s
	gcc -o test test.s src/lib/main.c src/lib/object.c
	./test

clean:
	rm -rf $(BIN)
