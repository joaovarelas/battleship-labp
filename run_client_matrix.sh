#!/bin/sh

reset && make clean client CMACRO=-D_MATRIX_ && make run
