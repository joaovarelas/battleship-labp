#!/bin/sh

reset && make clean client CMACRO=-DQUADTREE && make run
