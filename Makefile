CC=g++
CFLAGS = `pkg-config --cflags opencv4`
LIBS = `pkg-config --libs opencv4`

all: run

run: main
	@./main

main: main.cpp
	@$(CC) $(CFLAGS) $(LIBS) -o main main.cpp

clean:
	@rm main 


