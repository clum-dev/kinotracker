#!/bin/bash
clear && clear && make clean && make && clear && clear && echo "[RUN - DEBUG]" && echo -e
valgrind -s --leak-check=full --track-origins=yes --show-leak-kinds=all ./kinotracker $1