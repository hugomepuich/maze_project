#!/bin/bash

gcc main.c $(sdl2-config --cflags --libs) -lSDL_image -o output -Wall
./output