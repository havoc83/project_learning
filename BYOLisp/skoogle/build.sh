#!/bin/bash

cc -std=c99 -Wall skoogle.c mpc/mpc.c -ledit -lm -o bin/skoogle
