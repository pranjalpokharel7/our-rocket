CC=g++
FLAGS=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
OUTPUT=output
OBJ_DIR=object_files
SRC_DIR=source_files
OBJ_FILES=main.o shader.o glad.o

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)
vpath %.o $(OBJ_DIR)

output: $(OBJ_FILES)
	$(CC) $^ -o $@ $(FLAGS)

$(OBJ_DIR)/main.o : main.cpp shader.cpp
	$(CC) -c $< -o $@

$(OBJ_DIR)/shader.o : shader.cpp
	$(CC) -c $^ -o $@

$(OBJ_DIR)/glad.o : glad.c
	$(CC) -c $^ -o $@

.PHONY : clean
clean:
	rm -rf $(OBJ_FILES) $(OUTPUT)
