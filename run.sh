#!/bin/sh

reset && cd src/ && make clean all && cd ../ && ./bin/client
