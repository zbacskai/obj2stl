all:
	g++ -g -std=c++11 -I include src/main.cpp src/VertexData.cpp src/ObjFile.cpp src/TriangleModel.cpp -o test

clean:
	rm -rf test
