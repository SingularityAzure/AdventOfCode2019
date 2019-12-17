CC = g++ -O2 -DNDEBUG
CFLAGS = -std=c++17 -L/home/singularity/lib -lAzCoreLinux

.PHONY: all day01 day02 day03 day04 day05 day06 day07 day08 day09 day10 day11 day12 day13 day14 day15 day16 day17

all: day01 day02 day03 day04 day05 day06 day07 day08 day09 day10 day11 day12 day13 day14 day15 day16 day17

day01: Day01/main.cpp
	$(CC) Day01/main.cpp $(CFLAGS) -o Day01/day01
	Day01/day01

day02: Day02/main.cpp
	$(CC) Day02/main.cpp $(CFLAGS) -o Day02/day02
	Day02/day02

day03: Day03/main.cpp Day03/movements.c
	$(CC) Day03/main.cpp $(CFLAGS) -o Day03/day03
	Day03/day03

day04: Day04/main.cpp
	$(CC) Day04/main.cpp $(CFLAGS) -o Day04/day04
	Day04/day04

day05: Day05/main.cpp
	$(CC) Day05/main.cpp $(CFLAGS) -o Day05/day05
	Day05/day05

day06: Day06/main.cpp
	$(CC) Day06/main.cpp $(CFLAGS) -o Day06/day06
	Day06/day06

day07: Day07/main.cpp
	$(CC) Day07/main.cpp $(CFLAGS) -o Day07/day07
	Day07/day07

day08: Day08/main.cpp
	$(CC) Day08/main.cpp $(CFLAGS) -o Day08/day08
	Day08/day08

day09: Day09/main.cpp
	$(CC) Day09/main.cpp $(CFLAGS) -o Day09/day09
	Day09/day09

day10: Day10/main.cpp
	$(CC) Day10/main.cpp $(CFLAGS) -o Day10/day10
	Day10/day10

day11: Day11/main.cpp
	$(CC) Day11/main.cpp $(CFLAGS) -o Day11/day11
	Day11/day11

day12: Day12/main.cpp
	$(CC) Day12/main.cpp $(CFLAGS) -o Day12/day12
	Day12/day12

day13: Day13/main.cpp
	$(CC) Day13/main.cpp $(CFLAGS) -o Day13/day13
	Day13/day13

day14: Day14/main.cpp
	$(CC) Day14/main.cpp $(CFLAGS) -o Day14/day14
	Day14/day14

day15: Day15/main.cpp
	$(CC) Day15/main.cpp $(CFLAGS) -o Day15/day15
	Day15/day15

day16: Day16/main.cpp
	$(CC) Day16/main.cpp $(CFLAGS) -o Day16/day16
	Day16/day16

day17: Day17/main.cpp
	$(CC) Day17/main.cpp $(CFLAGS) -o Day17/day17
	Day17/day17
