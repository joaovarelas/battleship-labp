CC = gcc
FLAGS = -Wall -std=c99 -g

CLIENT = client
SERVER = server

SRC = player.c board.c game.c ship.c settings.c random.c cell.c quadtree.c

CLI = client.c
SRV = server.c

BINPATH = ../bin


${CLIENT}:
	${CC} ${FLAGS} -o ${BINPATH}/${CLIENT} ${CLI} ${SRC}


${SERVER}:
	${CC} ${FLAGS} -o ${BINPATH}/${SERVER} ${SRV} ${SRC}


all: ${CLIENT} ${SERVER}

clean:
	rm -f ${CLIENT} ${SERVER}

run:
	./${BINPATH}/${SERVER} ${ARGS}
	./${BINPATH}/${CLIENT} ${ARGS}
