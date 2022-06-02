CXX := g++
EXE := perky_chess
OPTIONS := -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
COMPILE_OPTS := #-g

SOURCE_FILES := $(wildcard src/*.cpp)
OBJECT_FILES := $(patsubst src/%.cpp,%.o,$(SOURCE_FILES))

all: $(EXE)

$(EXE): $(OBJECT_FILES)
	$(CXX) $(OBJECT_FILES) -o $(EXE) $(OPTIONS)

%.o: src/%.cpp
	$(CXX) -c $(COMPILE_OPTS) src/$*.cpp

valgrind:
	valgrind ./$(EXE)

clean:
	rm -f *.o $(EXE) vgcore.*

help:
	@echo make ---------- Compile and link
	@echo make clean ---- Remove all object files 
	@echo make help ----- Show this help menu