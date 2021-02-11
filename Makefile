all: main

main:
	$(CC) src/*.c -o main -O2

asan:
	$(CC) src/*.c -o main -O2 -fsanitize=address

clean:
	rm main -f

