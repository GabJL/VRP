all:
	(cd code; make all)

clean:
	(cd code; make clean)
	(cd res; rm *.txt)
	rm RS HC1 HC2 HC3 VNS ssGA *.txt
