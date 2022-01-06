LDFLAGS=-lcurses

all:
	g++ -o tinytetris tinytetris-commented.cpp src/*.cpp -lncurses

clean:
	rm -f tinytetris tinytetris-commented
