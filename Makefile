all:
	(cd code; make all)

clean:
	(cd code; make clean)
	(cd res; rm *.txt)
	rm RS HC VNS ssGA 
