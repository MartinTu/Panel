CC = g++
#SOURCES = $(shell find $(SOURCEDIR) -name '*.c')
#OBJECTS = $(SOURCES:%.c=%.o)
OBJECTS = main.o Animation.o Canvas.o Color.o Display.o DisplayModul.o Layer.o MyException.o Painting.o Server.o ServerDisplayHandler.o Spi.o Utils.o
LIBS = -lpthread -lwiringPi
CFLAGS = -g3 -Wall -D__GXX_EXPERIMENTAL_CXX0X__=1 -std=c++0x -pedantic
CFLAGSDBG = -g3 -Wall -D__GXX_EXPERIMENTAL_CXX0X__=1 -std=c++0x -pedantic -g
PROGRAMDIR = led/
BINDIR = /usr/bin/$(PROGRAMDIR)

RUNLVLSCRIPT = /etc/init.d/autoLedPanel
NAME = ledPanel
INSTALL = ledPanel panel_config.xml

TESTOBJECTS = $(patsubst %.o,obj/%.o,$(OBJECTS))

all: $(TESTOBJECTS)
	@echo -n "[ ][Link   ] $(NAME)"
	@$(CC) -o $(NAME) $(TESTOBJECTS) $(LIBS)
	@echo -n "\r[x][Link   ] $(NAME)\n"

obj/%.o: src/%.cpp
	@echo -n "[ ][Compile] $<"
	@$(CC) -c $(CFLAGSDBG) $< -o $@
	@echo -n "\r[x][Compile] $<\n"

clean:
	@echo -n "[ ][Clean  ] obj/*.o $(NAME)"
	@rm obj/*.o $(NAME) 
	@echo -n "\r[x][Clean  ] obj/*.o $(NAME)\n"

start:
	@echo -n "[ ][Start  ] $(NAME)"
	@$(RUNLVLSCRIPT) start
	@echo -n "\r[x][Start  ] $(NAME)                                                 \n"

stop:
	@echo -n "[ ][Stop   ] $(NAME)"
	@$(RUNLVLSCRIPT) stop
	@echo -n "\r[x][Stop   ] $(NAME)                                                  \n"

restart:
	@echo -n "[ ][Restart] $(NAME)"
	@$(RUNLVLSCRIPT) restart
	@echo -n "\r[x][Restart] $(NAME)                                                  \n"

install:
	@echo -n "[ ][Install] $(INSTALL) to $(BINDIR)"
	@if [ ! -d $(BINDIR) ] ; then \
    @mkdir $(BINDIR) ; fi;
	@install --mode=755 $(INSTALL) $(BINDIR)
	@echo -n "\r[x][Install] $(INSTALL) to $(BINDIR)\n"


uninstall:
	@echo -n "[ ][Uninst ] $(INSTALL) from $(BINDIR)"
	@rm $(BINDIR)$(NAME)
	@echo -n "\r[x][Uninst ] $(INSTALL) from $(BINDIR)\n"