#Error handling how to:
#-- see https://stackoverflow.com/questions/32222204/keep-going-turns-a-fatal-build-into-a-successful-build
# AND
#-- see https://makefiletutorial.com/
#
#On the command line --
#	Add -k when running make to continue running even in the face of errors.#		Helpful if you want to see all the errors of Make at once.
#	Add -i to make to have this happen for every command.
#
#Inside make -- 
#	Add a - before a command to suppress the error
#
CC = gcc

dummy: dummy.o

#Even if rm fails the - at the beginning tells make to keep going
clean:
	-rm -f *.o dummy
