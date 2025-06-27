all: build run

build:
	@cc main.c data.c -o wordle

run:
	@./wordle

clean:
	@rm -rf wordle
