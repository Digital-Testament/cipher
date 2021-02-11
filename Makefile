all: main

main:
	$(CC) src/* -o main -O2 -ansi

asan:
	$(CC) src/* -o main -O2 -ansi -fsanitize=address

clean:
	rm main -f

