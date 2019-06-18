CC = gcc 
WARNING_FLAGS = -Wall -Wextra -g
EXE = 537make
SCAN_BUILD_DIR = scan-build-out

all: main.o graphBuilder.o fileParser.o runCmd.o outOfDate.o detectCycle.o
	$(CC) -o $(EXE) main.o graphBuilder.o fileParser.o runCmd.o outOfDate.o detectCycle.o

main.o: main.c constants.h
	$(CC) $(WARNING_FLAGS) -c main.c

graphBuilder.o: graphBuilder.c graphNode.h
	$(CC) $(WARNING_FLAGS) -c graphBuilder.c

fileParser.o: fileParser.c graphNode.h
	$(CC) $(WARNING_FLAGS) -c fileParser.c

runCmd.o: runCmd.c
	$(CC) $(WARNING_FLAGS) -c runCmd.c

outOfDate.o: outOfDate.c
	$(CC) $(WARNING_FLAGS) -c outOfDate.c

detectCycle.o: detectCycle.c
	$(CC) $(WARNING_FLAGS) -c detectCycle.c

	
clean:
	rm $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

#
# Run the Clang Static Analyzer
#
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#
# View the one scan available using firefox
#
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html 