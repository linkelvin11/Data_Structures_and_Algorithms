spellcheck.exe: spellcheck.o hash.o
	g++ -o spellcheck.exe spellcheck.o hash.o

spellcheck.o: spellcheck.cpp
	g++ -c spellcheck.cpp

hash.o: hash.cpp hash.h
	g++ -c hash.cpp

debug:
	g++ -g -o spellcheck.exe spellcheck.cpp hash.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp makefile backups
