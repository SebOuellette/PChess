CXX := g++
EXE := perky_chess
#HEADERS_DIR := headers
SOURCE_DIR := src
LIBRARIES_DIR := libraries
LIBRARIES_LIB := lib
LIBRARIES_INCLUDE := include

CURRENT_DIR := $(shell pwd)
LINK_OPTIONS := -Wl,-rpath,"$(wildcard $(CURRENT_DIR)/$(LIBRARIES_DIR)/*/$(LIBRARIES_LIB))" -L$(wildcard $(CURRENT_DIR)/$(LIBRARIES_DIR)/*/$(LIBRARIES_LIB)) -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
COMPILE_OPTS := -I$(wildcard $(CURRENT_DIR)/$(LIBRARIES_DIR)/*/$(LIBRARIES_INCLUDE)) -g

SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECT_FILES := $(patsubst $(SOURCE_DIR)/%.cpp,%.o,$(SOURCE_FILES))

all: $(EXE)

$(EXE): $(OBJECT_FILES)
	$(CXX) $(OBJECT_FILES) -o $(EXE) $(LINK_OPTIONS)

%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) -c $(COMPILE_OPTS) $(SOURCE_DIR)/$*.cpp

clean:
	rm -f *.o $(EXE) vgcore.*

rebuild: clean all

help:
	@echo make ---------- Compile and link
	@echo make clean ---- Remove all object files
	@echo make rebuild -- Same as \`"make clean ; make"\` - Cleaning is required to \"rebuild\" header files
	@echo make help ----- Show this help menu
