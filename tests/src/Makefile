tests: tests.o cppvector.o
	g++ "../bin/tests.o" "../../bin/cppvector.o" -o tests -Ofast -Wall
tests.o:
	g++ -c "tests.cpp" -o "../bin/tests.o" -Ofast -Wall
cppvector.o:
	cd "../../src" && make
clean:
	rm -f ../bin/*.o
	rm -f tests
	rm -f tests.exe
	cd "../../src" && make clean