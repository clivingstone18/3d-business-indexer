LIBS = -lm

map2: main.o read.o ll.o kd.o dict.o utils.o data.o
	gcc -Wall -o map2 main.o read.o ll.o kd.o dict.o utils.o data.o -g -lm

main.o: main.c read.h
	gcc -Wall -c -o main.o main.c -g

read.o: read.c read.h
	gcc -Wall -c -o read.o read.c -g
    
ll.o: ll.c ll.h
	gcc -Wall -c -o ll.o ll.c -g

kd.o: kd.c kd.h
	gcc -Wall -c -o kd.o kd.c -g -lm

dict.o: dict.c dict.h
	gcc -Wall -c -o dict.o dict.c -g

utils.o: utils.c utils.h
	gcc -Wall -c -o utils.o utils.c -g

data.o: data.c data.h
	gcc -Wall -c -o data.o data.c -g
