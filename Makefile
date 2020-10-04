APP:=obj-convert
SRC_DIR:=./src
CXXFLAGS:= -g -std=c++11 -pedantic -Wall -Wextra -Werror
CPPFLAGS:= -I include
SOURCES:= $(wildcard ./src/*.cpp)
HEADERS:= $(wildcard ./include/*.hpp)
OBJS:= $(patsubst %.cpp, %.o, $(SOURCES))
LDFLAGS:=-lboost_program_options

%.o: %.cpp %.hpp
	g++ $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

obj-convert: $(OBJS)
	g++ $(CXXFLAGS) $(CPPFLAGS) $(OBJS) $(LDFLAGS) -o $@

all: $(APP)

check:
	cppclean --verbose .

style:
	astyle --style=allman $(SOURCES) $(HEADERS)

clean:
	rm -rf src/*.o
	rm -rf $(APP)
