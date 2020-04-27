#!/bin/sh

reset && cd src/ && make clean server && cd ../ && ./bin/server
