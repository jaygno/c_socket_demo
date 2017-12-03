CC = gcc
CFLAGS = -O2 -Wall
executables := $(patsubst %.c,%,$(wildcard *.c))  
                               
all : $(executables)  
	  
clean :    
	@rm -rf *.o *~    
	@rm -rf ${executables}    
.PHONY : clean  
