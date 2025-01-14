CC = gcc
CFLAGS = -Wall -g
aisle_FILES = aisle_manager.o \
	      .aisle_util.o   \
	      store_util.o    \
	      aisle_test.c
store_FILES = aisle_manager.o \
	      .aisle_util.o   \
	      .store_util.o   \
	      store_util.o    \
	      store_manager.c \
	      store_test.c

all: store_test aisle_test

aisle_manager.o: aisle_manager.c
	@echo "Building aisle_manager.c"
	@$(CC) $(CFLAGS) -c $< -o $@

store_util.o: store_util.c
	@$(CC) $(CFLAGS) -c $< -o $@

store_test: $(store_FILES)
	@echo "Building store_test"
	@$(CC) $(CFLAGS) -Wconversion $(store_FILES) -o $@

aisle_test: $(aisle_FILES)
	@echo "Building aisle_test"
	@$(CC) $(CFLAGS) $(aisle_FILES) -o $@

clean:
	@echo "Cleaning/resetting build files"
	@$(RM) aisle_test store_test aisle_manager.o store_util.o
	@$(RM) tests/*_diff*.txt tests/*_actual*.txt
