CC = g++
#SOURCES = $(shell find $(SOURCEDIR) -name '*.c')
#OBJECTS = $(SOURCES:%.c=%.o)
OBJECTS = main.o Utile.o Server.o ServerDisplayHandler.o Display.o MyException.o Share.o spi.o  DisplayModul.o colorManipulation.o
LIBS = -lpthread
CFLAGS = -g3 -Wall -D__GXX_EXPERIMENTAL_CXX0X__=1 -std=c++0x -pedantic
BINDIR = $(DESTDIR)/usr/bin
NAME = ledPanel
INSTALL = ledPanel panel_config.xml

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
	@echo 'Installing: $(INSTALL)'
	@install --mode=555 $(INSTALL) $(BINDIR)/
	@chmod u+x $(BINDIR)/$(NAME)
	@echo 'Finished Installation: $(INSTALL)'
	@echo ' '

uninstall:
	@echo 'Uninstalling: $(INSTALL) in $(BINDIR)/'
	@rm $(BINDIR)/$(INSTALL)
	@echo 'Finished: Uninstalling $(INSTALL)'
	@echo ' '

clean:
	@echo 'Cleaning: obj/*.o $(NAME)'
	@rm obj/*.o $(NAME) 
	@echo 'Finished: Cleaning'
	@echo ' '