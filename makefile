build:
	@gcc main.c -o main -lcurl
clean:
	@rm main
run:
	@./main
