TARGET = freetype_test
CC=g++ -Wall
all:
	$(CC) -o $(TARGET) $(TARGET).cpp -lfreetype -lm -I/usr/include/freetype2

