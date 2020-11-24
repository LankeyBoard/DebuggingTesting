roll: 
	g++ Washington_FaultSeeded.cpp -o roll.exe
testroll:
	g++ -o testroll.exe classtests.cpp 
all: roll testroll
clean:
	-rm testroll.exe
	-rm roll.exe
	