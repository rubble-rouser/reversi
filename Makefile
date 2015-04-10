SHELL = /bin/bash

CXX ?= ccache g++
CXXFLAGS = -Wall -Wextra -g
LDFLAGS = -Ldeps/SFML/lib/ -lsfml-audio -lsfml-graphics -lsfml-network \
	-lsfml-system -lsfml-window
INCLUDES = -Ideps/SFML/include

BLDDIR = build

REVERSI_SRCS = ai.cpp board.cpp game.cpp main.cpp
REVERSI_OBJS = $(patsubst %.cpp, $(BLDDIR)/%.o, $(REVERSI_SRCS))

all: $(patsubst %, $(BLDDIR)/%, reversi)

clean:
	rm -rf $(BLDDIR)

sfml:
	cd deps/SFML; \
	git clean -dfx; \
	cmake -D CMAKE_BUILD_TYPE="Release" \
	  -D CMAKE_CXX_COMPILER="/usr/lib/ccache/bin/g++" \
	  -D CMAKE_C_COMPILER="/usr/lib/ccache/bin/gcc" \
	  -D BUILD_SHARED_LIBS=TRUE ./; \
	# cmake ./; \
	make

$(BLDDIR)/reversi: $(REVERSI_OBJS)

$(BLDDIR)/%:
	$(CXX) $^ $(LDFLAGS) -o $@

$(BLDDIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

test: $(BLDDIR)/reversi
	LD_LIBRARY_PATH=deps/SFML/lib ./$(BLDDIR)/reversi

.PHONY: all clean sfml test
