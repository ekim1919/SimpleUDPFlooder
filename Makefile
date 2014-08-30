EXEC=sup
CC=c99
CFLAGS= -c -g -Wall
OBJECTS=simpleudpflood.o
LIB=-lnet

all: $(EXEC)
	make clean
$(EXEC): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $(EXEC) $(LIB)

simpleudpflood.o:
	$(CC) $(CFLAGS) simpleudpflood.c
clean:
	rm -f *.o     
