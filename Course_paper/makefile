.PHONY: all clean 
CFLAGS = -Wall -Wextra
CPPFLAGS = -MMD

bin/fsmatcher: obj/fsmatcher.o obj/functions.o
	gcc $(CFLAGS) -g -o $@ $^


obj/functions.o: src/functions.c
	gcc -c $(CFLAGS) $(CPPFLAGS) -I src/ -o $@ src/functions.c

obj/fsmatcher.o: src/fsmatcher.c
	gcc -c $(CFLAGS) $(CPPFLAGS) -I src/ -o $@ src/fsmatcher.c

run/fsmatcher:
	./bin/fsmatcher


clean:
	rm obj/*/*.[od]

