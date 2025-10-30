CC=gcc
CFLAGS=-std=c17 -Wall -Wextra -Wundef -Wshadow -D_POSIX_C_SOURCE=200809L -Iheaders
TARGET=executable

$(TARGET): main.o connection.o persistency.o
	${CC} ${CFLAGS} main.o connection.o persistency.o -o $(TARGET)

main.o: main.c
	${CC} ${CFLAGS} -c main.c -o main.o

connection.o: headers/connection.h connection.c
	${CC} ${CFLAGS} -c connection.c -o connection.o

persistency.o: headers/persistency.h persistency.c
	${CC} ${CFLAGS} -c persistency.c -o persistency.o

clean :
	rm -f *.o $(TARGET)

run : $(TARGET)
	./$(TARGET)

debug : program.elf
	gdb $(TARGET)
