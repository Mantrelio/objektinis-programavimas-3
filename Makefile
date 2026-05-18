CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
TEST_CXXFLAGS = $(CXXFLAGS) -Ivendor/catch2

TARGET = main
TEST_TARGET = test
OBJDIR = build/obj
TEST_OBJDIR = build/test-obj

APP_SOURCES = src/main.cpp src/main-menu.cpp src/utils/input-utils.cpp \
	src/utils/grading-utils.cpp src/student.cpp \
	src/generate-student-list.cpp \
	src/test/runtime-test/generate-student-list-test.cpp \
	src/test/runtime-test/output-nuskriausti-and-protingi-test.cpp \
	src/test/runtime-test/student-rule-of-five-operators-test.cpp \
	src/test/runtime-test/custom-vector-push-back-test.cpp \
	src/test/runtime-test/custom-vector-reallocation-test.cpp \
	src/data-sorting/sort-students.cpp \
	src/data-sorting/sort-nuskriausti-and-protingi.cpp \
	src/output-results.cpp \
	src/create-student.cpp
APP_OBJECTS = $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(APP_SOURCES))

TEST_APP_SOURCES = src/student.cpp src/utils/grading-utils.cpp
TEST_OBJECTS = $(patsubst src/%.cpp,$(TEST_OBJDIR)/%.o,$(TEST_APP_SOURCES)) \
	$(TEST_OBJDIR)/vendor/catch2/catch_amalgamated.o \
	$(TEST_OBJDIR)/test/catch2-main.o \
	$(TEST_OBJDIR)/test/unit-test/student-catch2-test.o \
	$(TEST_OBJDIR)/test/unit-test/custom-vector-catch2-test.o

CUSTOM_VECTOR_HEADERS = include/custom-vector.h include/custom-vector.tpp

$(TARGET): $(APP_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(APP_OBJECTS)

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(TEST_CXXFLAGS) -o $@ $(TEST_OBJECTS)

$(OBJDIR)/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/test/runtime-test/custom-vector-push-back-test.o: \
		src/test/runtime-test/custom-vector-push-back-test.cpp $(CUSTOM_VECTOR_HEADERS)

$(OBJDIR)/test/runtime-test/custom-vector-reallocation-test.o: \
		src/test/runtime-test/custom-vector-reallocation-test.cpp $(CUSTOM_VECTOR_HEADERS)

$(TEST_OBJDIR)/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(TEST_CXXFLAGS) -c $< -o $@

$(TEST_OBJDIR)/test/unit-test/custom-vector-catch2-test.o: \
		src/test/unit-test/custom-vector-catch2-test.cpp $(CUSTOM_VECTOR_HEADERS)

$(TEST_OBJDIR)/vendor/catch2/%.o: vendor/catch2/%.cpp
	mkdir -p $(@D)
	$(CXX) $(TEST_CXXFLAGS) -c $< -o $@

all: $(TARGET) $(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)
	rm -rf $(OBJDIR) $(TEST_OBJDIR)

.PHONY: all clean
