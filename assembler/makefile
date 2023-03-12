MAIN_FLAG = gcc -g -Wall -ansi -pedantic

OBJ_FLAG = gcc -c -Wall -ansi -pedantic

OBJECTS = assembler.o utils.o pre_assembler.o constants.o data_structure.o first_pass.o second_pass.o parse.o errors.o output.o

HEADERS = assembler.h utils.h pre_assembler.h constants.h data_structure.h first_pass.h second_pass.h parse.h errors.h output.h

assembler: $(OBJECTS)
	$(MAIN_FLAG) $(OBJECTS) -o assembler

assembler.o: assembler.c $(HEADERS)
	$(OBJ_FLAG) assembler.c -o assembler.o

constants.o: constants.c $(HEADERS)
	$(OBJ_FLAG) constants.c -o constants.o

data_structure.o: data_structure.c constants.h $(HEADERS)
	$(OBJ_FLAG) data_structure.c -o data_structure.o

pre_assembler.o: pre_assembler.c $(HEADERS)
	$(OBJ_FLAG) pre_assembler.c -o pre_assembler.o

first_pass.o: first_pass.c $(HEADERS)
	$(OBJ_FLAG) first_pass.c -o first_pass.o

second_pass.o: second_pass.c $(HEADERS)
	$(OBJ_FLAG) second_pass.c -o second_pass.o

parse.o: parse.c $(HEADERS)
	$(OBJ_FLAG) parse.c -o parse.o

errors.o: errors.c $(HEADERS)
	$(OBJ_FLAG) errors.c -o errors.o

utils.o: utils.c $(HEADERS)
	$(OBJ_FLAG) utils.c -o utils.o

output.o: output.c $(HEADERS)
	$(OBJ_FLAG) output.c -o output.o

debug: $(OBJECTS)
	$(MAIN_FLAG) $(OBJECTS) -o assembler-debug

clean:
	rm -f *.o
