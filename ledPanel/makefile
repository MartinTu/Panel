CC = g++
OBJECTS = main.o Server.o ServerDisplayHandler.o Display.o MyException.o Share.o spi.o Utile.o
LIBS = -lpthread
CFLAGS = -g3 -Wall -D__GXX_EXPERIMENTAL_CXX0X__=1 -std=c++0x -pedantic
BINDIR = $(DESTDIR)/usr/bin
NAME = ledPanel

TESTOBJECTS = $(patsubst %.o,obj/%.o,$(OBJECTS))

cbg: $(TESTOBJECTS)
	$(CC) -o $(NAME) $(TESTOBJECTS) $(LIBS)


obj/%.o: src/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@


install:
	install --mode=755 $(NAME) $(BINDIR)/

clean:
	rm $(NAME) obj/*.o


uninstall:
	rm $(BINDIR)/$(NAME)
