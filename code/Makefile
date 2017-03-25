all: RS HC VNS ssGA
	cp RS HC VNS ssGA ..

clean:
	rm RS HC VNS ssGA

RS: RS.cpp cInstance.cpp cConfiguration.cpp
	g++ -o RS RS.cpp cInstance.cpp cConfiguration.cpp -std=c++11

HC: HC.cpp cInstance.cpp
	g++ -o HC HC.cpp cInstance.cpp cConfiguration.cpp -std=c++11

VNS: VNS.cpp cInstance.cpp
	g++ -o VNS VNS.cpp cInstance.cpp 

ssGA: ssGA.cpp cInstance.cpp
	g++ -o ssGA ssGA.cpp cInstance.cpp 
