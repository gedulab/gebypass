all: gebypass gebypass-asan gebypass-asan-wa

gebypass: gebypass.c
	gcc -g -O2 -o $@ $^


gebypass-asan: gebypass.c
	gcc -g -fsanitize=address -lasan -O2 -o $@ $^


gebypass-asan-wa: gebypass.c
	gcc -g -DSANITIZER_CAN_USE_ALLOCATOR64=0 -fsanitize=address -lasan -O2 -o $@ $^

clean:
	rm gebypass gebypass-asan gebypass-asan-wa