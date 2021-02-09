all:
	g++ -Wall -Werror -std=c++17 -o main *.cpp Lexer/*.h Parser/*.h  -g
clean:
	rm main
