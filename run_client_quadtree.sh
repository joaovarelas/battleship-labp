#!/bin/sh

reset && make clean client CMACRO=-D_QUADTREE_ && make run
