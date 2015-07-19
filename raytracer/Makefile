CXX_FLAGS = -Wall

SOURCE_DIR = src

TEST_DIR = test

RELEASE_BIN = release_bin

TEST_BIN = test_bin

GTEST_DIR = gtest-1.5.0

GTEST_MAKE_DIR = $(GTEST_DIR)/make

RT_OBJS = driver.o

TEST_OBJS = alltests.o

raytracer: $(RT_OBJS)
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) $(RT_OBJS) -o raytracer

all: raytracer draytracer unittests docs

docs:
	doxygen

# for debugging. It has the -g flag. test is for unit tests
draytracer: $(RT_OBJS)
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -g $(RT_OBJS) -o draytracer 

unittests: $(GTEST_OBJS) $(TEST_OBJS)
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -g \
	$(TEST_OBJS) $(GTEST_MAKE_DIR)/gtest-all.o -lpthread -o unittests
	
%.o: $(SOURCE_DIR)/%.cc
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -c $< -o $@
	
%.o: $(TEST_DIR)/%.cc
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -I$(GTEST_DIR)/include -Isrc -c $< -o $@
	
gtest-all.o:
	make -C $(GTEST_MAKE_DIR)

clean:
	rm -rf docs raytracer draytracer unittests *~ *.o

clean_crap:
	rm -rf *~ *.o

.PHONY: clean clean_crap