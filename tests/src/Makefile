tests: tests.o cvector.o
	cc "../bin/tests.o" "../../bin/cvector.o" -o tests
tests.o:
	cc -c "tests.c" -o "../bin/tests.o"
cvector.o:
	cd "../../src" && make