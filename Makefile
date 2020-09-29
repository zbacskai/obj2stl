APP:=obj-convert
SRC_DIR:=./src
CXXFLAGS:= -g -std=c++11
CPPFLAGS:= -I include
SOURCES:= $(wildcard ./src/*.cpp)
OBJS:= $(patsubst %.cpp, %.o, $(SOURCES))
LDFLAGS:=-lboost_program_options

%.o: %.cpp
	g++ $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

obj-convert: $(OBJS)
	g++ $(CXXFLAGS) $(CPPFLAGS) $(OBJS) $(LDFLAGS) -o $@

all: $(APP)

clean:
	rm -rf src/*.o
	rm -rf $(APP)
