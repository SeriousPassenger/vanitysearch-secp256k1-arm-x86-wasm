# Minimal Makefile — Emscripten toolchain only
# ----------------------------------------------------------
CXX        = em++
CXXFLAGS   = -O3 -Wall -Wextra -I../include \
             -s STANDALONE_WASM=1           \
             -s INITIAL_MEMORY=64MB         \
             -s MAXIMUM_MEMORY=1GB          \
             -s WASM=1
LDFLAGS    = -L../ -lstdc++ -lsecp256k1_wasm

TARGET     = main.wasm
SRCS       = test_int_point.cpp
OBJS       = $(SRCS:.cpp=.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	wasmtime run --dir=. $(TARGET) -- $(ARGS)

clean:
	rm -f $(TARGET) $(OBJS)
