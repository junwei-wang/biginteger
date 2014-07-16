OBJs= test#run
all: $(OBJs) 
test: bignum.o rsa.o
bignum.o: bignum.h
rsa.o: rsa.h

.PHONY: clean run
run:
	time ./test
clean:
	- rm $(OBJs)
	- rm *.o
