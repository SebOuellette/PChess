CXX := g++
EXE := perky_chess
LINK_OPTIONS := -Wl,-rpath,"./libraries/SFML-2.5.1-64/lib" -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system  
COMPILE_OPTS := -I./libraries/SFML-2.5.1-64/include #-g 

SOURCE_FILES := $(wildcard src/*.cpp)
OBJECT_FILES := $(patsubst src/%.cpp,%.o,$(SOURCE_FILES))

all: $(EXE)

$(EXE): $(OBJECT_FILES)
	$(CXX) $(OBJECT_FILES) -o $(EXE) $(LINK_OPTIONS)

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