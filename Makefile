CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -D_POSIX_C_SOURCE=200809L
INCLUDES = -Iinclude
SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/parking

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

run: $(TARGET)
	./$(TARGET)

test: $(OBJDIR)/test_engine.o $(filter-out $(OBJDIR)/main.o, $(OBJECTS))
	$(CC) $(OBJDIR)/test_engine.o $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) -o $(BINDIR)/test_engine
	./$(BINDIR)/test_engine

$(OBJDIR)/test_engine.o: test_engine.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c test_engine.c -o $(OBJDIR)/test_engine.o
