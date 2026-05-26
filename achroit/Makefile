CXX=g++
CXX_FLAGS= -g
CXX_SRC=$(shell find . -type f -name "*.cpp")
OBJ=$(patsubst %, %.o, $(CXX_SRC))
OUTPUT=achroit

$(OUTPUT): $(OBJ)
	@echo "Linking"
	@$(CXX) $(OBJ) -o $@ $(CXX_FLAGS)

%.cpp.o: %.cpp
	@echo "CXX $<"
	@$(CXX) -c $< -o $@ $(CXX_FLAGS)

dotest: $(OUTPUT)
	@./$< "./test"

clean:
	@rm -f $(OBJ) $(OUTPUT)