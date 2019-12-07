CC = g++ -O2
CFLAGS = -std=c++17 -L/home/singularity/lib -lAzCoreLinux

.PHONY: all day1 day2 day3 day4 day5 day6 day7

all: day1 day2 day3 day4 day5 day6 day7

day1: Day1/main.cpp
	$(CC) Day1/main.cpp $(CFLAGS) -o Day1/day1
	Day1/day1

day2: Day2/main.cpp
	$(CC) Day2/main.cpp $(CFLAGS) -o Day2/day2
	Day2/day2

day3: Day3/main.cpp Day3/movements.c
	$(CC) Day3/main.cpp $(CFLAGS) -o Day3/day3
	Day3/day3

day4: Day4/main.cpp
	$(CC) Day4/main.cpp $(CFLAGS) -o Day4/day4
	Day4/day4

day5: Day5/main.cpp
	$(CC) Day5/main.cpp $(CFLAGS) -o Day5/day5
	Day5/day5

day6: Day6/main.cpp
	$(CC) Day6/main.cpp $(CFLAGS) -o Day6/day6
	Day6/day6

day7: Day7/main.cpp
	$(CC) Day7/main.cpp $(CFLAGS) -o Day7/day7
	Day7/day7
