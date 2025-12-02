CC = gcc

TOP_SRCDIR = ../../..
SRC_DIR = $(TOP_SRCDIR)/source

CFLAGS = -I$(SRC_DIR) -I$(SRC_DIR)/include -Wall -Wextra -O2

LDFLAGS = \
    -L$(SRC_DIR)/.libs \
    -Wl,-rpath,'$$ORIGIN/../../../source/.libs' \
    -lprivilege \
    -lcap \
    -ljsoncpp \
    -lstdc++

TARGET = cap_test_process
SRC = cap_test_process.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o

