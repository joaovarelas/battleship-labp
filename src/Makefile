CLIENT = client
SERVER = server

SRC = player.c board.c game.c ship.c settings.c

CLI = client.c
SRV = server.c

GCCARGS = -Wall -std=c99

BINPATH = ../bin

${CLIENT}:
	gcc ${GCCARGS} -o ${BINPATH}/${CLIENT} ${CLI} ${SRC}


${SERVER}:
	gcc ${GCCARGS} -o ${BINPATH}/${SERVER} ${SRV} ${SRC}

all: ${CLIENT} ${SERVER}

clean:
	rm -f ${CLIENT} ${SERVER}

run:
	./${BINPATH}/${SERVER} ${ARGS}
	./${BINPATH}/${CLIENT} ${ARGS}
