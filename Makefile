CC = cc

CFLAGS = -Wall
LDFLAGS =

TARGET = ics_test

INCLUDEDIR = -I./include -I./
SOURCES = $(shell find . -name "*.c")
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all
all : $(TARGET) $(LIB)


$(TARGET) : $(OBJECTS)
	$(CC) $(notdir $^) $(LDFLAGS) -o $@


%.o : %.c
	$(CC) $(INCLUDEDIR) -c $(CFLAGS) $< -o $(notdir $@)


.PHONY: clean test

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o