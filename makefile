all:
	g++ -Wall -Werror -std=c++17 -o main *.h *.cpp -g
clean:
	rm main
