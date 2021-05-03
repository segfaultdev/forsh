#!/bin/sh

gcc -Iinclude $(find . -name "*.c") -o forsh -lm
