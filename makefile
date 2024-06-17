build:
	@gcc main.c -o main -lcurl -Wall
clean:
	@rm main
run:
	@./main
