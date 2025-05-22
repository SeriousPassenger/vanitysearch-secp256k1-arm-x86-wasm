CXX = g++
AR = ar
CXXFLAGS = -O3 -Wall -fPIC -std=c++11 -Wno-write-strings -Wno-strict-aliasing -Wno-unused-variable
INCLUDES = -I.

LIB_NAME = libsecp256k1_cpu.a
OBJ_DIR = obj

SOURCES = secp256k1.cpp \
          Point.cpp \
          IntMod.cpp \
          IntGroup.cpp \
          Int.cpp

OBJECTS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

.PHONY: all clean

all: $(LIB_NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(LIB_NAME): $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

clean:
	rm -rf $(OBJ_DIR) $(LIB_NAME)

# Install target (optional)
install: $(LIB_NAME)
	@mkdir -p $(DESTDIR)/lib
	@mkdir -p $(DESTDIR)/include/secp256k1
	cp $(LIB_NAME) $(DESTDIR)/lib/
	cp include/*.h $(DESTDIR)/include/secp256k1/
