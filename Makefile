CXX = g++
# originally made with CXX = g++-11
CXXFLAGS = -std=c++20 -Wall -MMD -Werror=vla -g
SRCS = $(shell find . -type f -name "*.cc")
OBJECTS = ${SRCS:.cc=.o}
DEPENDS = ${OBJECTS:.o=.d}
EXEC = cmd_catan

.PHONY: all clean

all: ${EXEC}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

-include ${DEPENDS}

clean:
	rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}
