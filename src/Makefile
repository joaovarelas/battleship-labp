CC = gcc
FLAGS = -Wall -std=c99 -D_GNU_SOURCE -g

CLIENT = client

SRC = client.c server.c player.c board.c game.c ship.c settings.c random.c cell.c quadtree.c

BINPATH = ../bin

${CLIENT}:
	${CC} ${FLAGS} -o ${BINPATH}/${CLIENT} ${CLI} ${SRC}

clean:
	rm -f ${BINPATH}/*

run:
	${BINPATH}/${CLIENT} ${ARGS}
