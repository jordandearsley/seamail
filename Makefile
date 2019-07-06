
TEST_DIR 	:= tests
TESTS 		:= $(shell find $(TEST_DIR) -name '*.cc')

all:
	export TEST_MODE=0
	export TEST_FILE=""
	$(MAKE) -f core.mk

.PHONY: force clean run

run:
	$(MAKE) -f core.mk run
clean:
	find $(TEST_DIR) -name '*.out' -delete -print
	find $(TEST_DIR) -name '*.vg' -delete -print
	$(MAKE) -f core.mk clean
	
$(TEST_DIR)/%.cc: force
	TEST_MODE=1 TEST_FILE=$@ $(MAKE) -f core.mk

test: $(TESTS)
	@echo completed test suite for all tests in $(TEST_DIR)...

