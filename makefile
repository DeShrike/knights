CC = gcc
LIBS = -lm -lpng
FLAGS = -O2 -Wall

all: knights

knights: knights.o image.o utils.o
	$(CC) $^ -o knights $(LIBS)

knights.o: knights.c
	$(CC) -c $< $(FLAGS)

image.o: image.c
	$(CC) -c $< $(FLAGS)

utils.o: utils.c
	$(CC) -c $< $(FLAGS)

clean:
	rm -v *.o knights
