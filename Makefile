LDFLAGS=-lcurses

all:
	g++ -o tinytetris tinytetris-commented.cpp -lncurses

clean:
	rm -f tinytetris tinytetris-commented
