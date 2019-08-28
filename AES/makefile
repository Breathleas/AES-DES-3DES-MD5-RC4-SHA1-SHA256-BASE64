#create bye yekai 20161101

SRC_FILES=$(wildcard *.cpp) ###get all .c files 
OBJ_FILES=$(patsubst %.cpp,%.o,$(SRC_FILES))
GCC=g++
IncPath=-I include
CFlags=-Wall -g

test:$(OBJ_FILES)
	$(GCC) -o $@ $^ $(CFlags)
%.o:%.c
	$(GCC) -c $? -o $@ $(IncPath) $(CFlags)

clean:
	-@rm -f test
	-@rm -f $(OBJ_FILES)
.PHONY:all clean

