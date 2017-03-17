all: RS HC1 HC2 HC3 VNS ssGA
	cp RS HC1 HC2 HC3 VNS ssGA ..

clean:
	rm RS HC1 HC2 HC3 VNS ssGA

RS: RS.cpp cInstance.cpp
	g++ -o RS RS.cpp cInstance.cpp

HC1: HC1.cpp cInstance.cpp
	g++ -o HC1 HC1.cpp cInstance.cpp

HC2: HC2.cpp cInstance.cpp
	g++ -o HC2 HC2.cpp cInstance.cpp

HC3: HC3.cpp cInstance.cpp
	g++ -o HC3 HC3.cpp cInstance.cpp

VNS: VNS.cpp cInstance.cpp
	g++ -o VNS VNS.cpp cInstance.cpp 

ssGA: ssGA.cpp cInstance.cpp
	g++ -o ssGA ssGA.cpp cInstance.cpp 
