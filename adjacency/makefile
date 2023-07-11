HEADERS_PATH = include/
SOURCE = src/*
HEADERS = $(HEADERS_PATH)
FLAGS = -Wall -ansi -pedantic
OBJECT = adjacency

$(OBJECT): $(SOURCE) $(HEADERS)
	gcc $(SOURCE) -I $(HEADERS_PATH) -o $(OBJECT) $(FLAGS)
