CLIENT = client
SERVER = server

SRC = player.c board.c game.c ship.c settings.c

CLISRC = client.c
SRVSRC = server.c

${CLIENT}: ${CLISRC} ${SRC}
	gcc -o ${CLIENT} ${CLISRC} ${SRC}


${SERVER}: ${SRVSRC} ${SRC}
	gcc -o ${SERVER} ${SRVSRC} ${SRC}

clean:
	rm -f ${CLIENT} ${SERVER}

run: ${CLIENT}	
	./${CLIENT} ${ARGS}
