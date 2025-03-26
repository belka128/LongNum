CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra

MAIN_SRC := main.cpp lexer.cpp parser.cpp
TEST_SRC := catch_tests.cpp lexer.cpp parser.cpp

TARGET := differentiator
TEST_TARGET := run_tests

.PHONY: all clean test

all: $(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TARGET): $(MAIN_SRC) expression.h lexer.h parser.h token.h
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) -o $@

$(TEST_TARGET): $(TEST_SRC) expression.h lexer.h parser.h token.h
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $@

clean:
	rm -f $(TARGET) $(TEST_TARGET)