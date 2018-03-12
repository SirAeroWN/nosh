HEADERS = internal_commands.h tokenizer.h
OBJECTS = nosh.o internal_commands.o tokenizer.o

default: nosh

%.o: %.c $(HEADERS)
	gcc -g -c $< -o $@

nosh: $(OBJECTS)
	gcc $(OBJECTS) -g -o $@ -lreadline -lncurses

clean:
	-rm -f $(OBJECTS)
	-rm -f nosh