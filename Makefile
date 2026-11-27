CC := cc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Iinclude

SRC := $(wildcard src/*.c)
HEADERS := $(wildcard include/*.h)

DEBUG_OBJS := $(SRC:src/%.c=build/debug/%.o)
STRESS_OBJS := $(SRC:src/%.c=build/stress/%.o)
ASAN_OBJS := $(SRC:src/%.c=build/asan/%.o)
FINAL_OBJS := $(SRC:src/%.c=build/final/%.o)

.DEFAULT_GOAL := debug

# Target specific flags
debug: EXTRA_CFLAGS = -g -O0
stress: EXTRA_CFLAGS = -g -O0 -DGC_STRESS
asan: EXTRA_CFLAGS = -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer
asan: EXTRA_LDFLAGS = -fsanitize=address,undefined
final: EXTRA_CFLAGS = -O2 -DNDEBUG

debug: build/debug/lispgc
stress: build/stress/lispgc
asan: build/asan/lispgc
final: build/final/lispgc

build/debug/lispgc: $(DEBUG_OBJS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $^ -o $@ $(EXTRA_LDFLAGS)
build/stress/lispgc: $(STRESS_OBJS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $^ -o $@ $(EXTRA_LDFLAGS)
build/asan/lispgc: $(ASAN_OBJS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $^ -o $@ $(EXTRA_LDFLAGS)
build/final/lispgc: $(FINAL_OBJS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $^ -o $@ $(EXTRA_LDFLAGS)

build/debug/%.o: src/%.c $(HEADERS) Makefile
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $< -o $@
build/stress/%.o: src/%.c $(HEADERS) Makefile
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $< -o $@
build/asan/%.o: src/%.c $(HEADERS) Makefile
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $< -o $@ 
build/final/%.o: src/%.c $(HEADERS) Makefile
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $< -o $@

test: debug stress asan
	@echo "=== Running tests with debug flags ==="
	./tests/run_tests.sh build/debug/lispgc
	@echo "=== Running tests with stress flags ==="
	./tests/run_tests.sh build/stress/lispgc
	@echo "=== Running tests with asan flags ==="
	./tests/run_tests.sh build/asan/lispgc

clean:
	rm -rf build
