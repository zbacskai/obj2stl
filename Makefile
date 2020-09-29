all:
	g++ -g -std=c++11 -I include src/main.cpp src/VertexData.cpp src/ObjFile.cpp src/TriangleModel.cpp src/StlAscii.cpp src/StlBin.cpp src/ModelConverter.cpp src/CheckPoint.cpp -lboost_program_options -o test

clean:
	rm -rf test
