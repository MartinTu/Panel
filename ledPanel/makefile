CC = g++
#SOURCES = $(shell find $(SOURCEDIR) -name '*.c')
#OBJECTS = $(SOURCES:%.c=%.o)
OBJECTS = main.o Utile.o Server.o ServerDisplayHandler.o Display.o MyException.o Share.o spi.o  DisplayModul.o colorManipulation.o
LIBS = -lpthread
CFLAGS = -g3 -Wall -D__GXX_EXPERIMENTAL_CXX0X__=1 -std=c++0x -pedantic
BINDIR = $(DESTDIR)/usr/bin
NAME = ledPanel

TESTOBJECTS = $(patsubst %.o,obj/%.o,$(OBJECTS))

all: $(TESTOBJECTS)
	@echo 'Linking: $@'
	@$(CC) -o $(NAME) $(TESTOBJECTS) $(LIBS)
	@echo 'Finished: $@'
	@echo ' '

obj/%.o: src/%.cpp
	@echo 'Compiling: $@'
	@$(CC) -c $(CFLAGS) $< -o $@
	@echo 'Finished: $@'
	@echo ' '

install:
	@echo 'Installing: $(NAME)'
	@install --mode=755 $(NAME) $(BINDIR)/
	@echo 'Finished Installation: $(NAME)'
	@echo ' '

uninstall:
	@echo 'Uninstalling: $(NAME) in $(BINDIR)/'
	@rm $(BINDIR)/$(NAME)
	@echo 'Finished: Uninstalling $(NAME)'
	@echo ' '

clean:
	@echo 'Cleaning: obj/*.o $(NAME)'
	@rm obj/*.o $(NAME) 
	@echo 'Finished: Cleaning'
	@echo ' '