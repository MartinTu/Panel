CC = g++
#SOURCES = $(shell find $(SOURCEDIR) -name '*.c')
#OBJECTS = $(SOURCES:%.c=%.o)
OBJECTS = main.o Utile.o Server.o ServerDisplayHandler.o Display.o MyException.o Share.o spi.o  DisplayModul.o colorManipulation.o Canvas.o Animation.o
LIBS = -lpthread -lwiringPi
CFLAGS = -g3 -Wall -D__GXX_EXPERIMENTAL_CXX0X__=1 -std=c++0x -pedantic
PROGRAMDIR = led/
BINDIR = /usr/bin/$(PROGRAMDIR)

RUNLVLSCRIPT = /etc/init.d/autoLedPanel
NAME = ledPanel
INSTALL = ledPanel panel_config.xml

TESTOBJECTS = $(patsubst %.o,obj/%.o,$(OBJECTS))

all: $(TESTOBJECTS)
	@echo "[Link  ] $(NAME)"
	@$(CC) -o $(NAME) $(TESTOBJECTS) $(LIBS)
	@echo 'Finished: $@'
	@echo ' '

obj/%.o: src/%.cpp
	@echo "[Compile] $@"
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@echo 'Cleaning: obj/*.o $(NAME)'
	@rm obj/*.o $(NAME) 
	@echo 'Finished: Cleaning'
	@echo ' '
	
start:
	@echo 'Starting: $(Name)'
	@$(RUNLVLSCRIPT) start
	
stop:
	@echo 'Stopping: $(Name)'
	@$(RUNLVLSCRIPT) stop

restart:
	@echo 'Restarting: $(Name)'
	@$(RUNLVLSCRIPT) restart

install:
	@echo 'Installing: $(INSTALL) to $(BINDIR)'	
	@if [ ! -d $(BINDIR) ] ; then \
    @mkdir $(BINDIR) ; fi;
	@install --mode=755 $(INSTALL) $(BINDIR)
	@echo 'Finished Installation: $(NAME)'
	@echo ' '

uninstall:
	@echo 'Uninstalling: $(NAME) from $(BINDIR)'
	@rm $(BINDIR)$(NAME)
	@echo 'Finished: Uninstalling $(NAME)'
	@echo ' '

