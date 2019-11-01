build: tema.c
	gcc -Wall tema.c -lncurses -o trivia

run_arguments: trivia
	./trivia quiz.txt

run_without_arguments: trivia
	./trivia 
clean: telefon
	rm -f trivia
