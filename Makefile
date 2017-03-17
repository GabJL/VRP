all:
	(cd code; make all)

clean:
	(cd code; make clean)
	rm RS HC1 HC2 HC3 VNS ssGA *.txt
