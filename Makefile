CC=g++

all: main

main: main.cpp
	@$(CC) -o main main.cpp

run: main
	@./main

clean:
	@rm main 



