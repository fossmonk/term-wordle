# term-wordle
This is an implementation of a terminal version of WORDLE in C. ANSI-C codes are used for the background and foreground color settings. So make sure colors are enabled in the terminal.

## Build and Run
- `make build` will generate the `wordle` binary
- `make run` will run the already generated binary
- `make` or `make all` will do build and run
- `make clean` will delete the `wordle` binary

## Details
- The word list for valid guesses and answers are stored in C string arrays, this is so that once compiled the binary doesn't need another data file (like JSON/yaml) to be run
- I have only tested this in a UNIX+BASH setup. Should work for other shells, and Windows too.

## Demo

https://github.com/user-attachments/assets/558afda0-e641-4236-84fb-4a329f686fd9

