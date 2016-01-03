#!/bin/bash
PROJECT_PATH="/home/kevin/gitrepos/CodeParser"
TEST_BINARY="$PROJECT_PATH/bin/test/test_binary"
TEST_DISASM="$PROJECT_PATH/bin/test/test_dump.disasm"
LD_PATH_ADD="$PROJECT_PATH/bin/lib/"

if [[ ! "$LD_LIBRARY_PATH" == *"$LD_PATH_ADD"* ]]; then
	printf "Exporting lib folder...\n"
	export LD_LIBRARY_PATH="$LD_PATH_ADD"
fi

if [[ ! -e "$TEST_DISASM" ]]; then
	if [[ ! -e "$TEST_BINARY" ]]; then
		printf "Test binary and objdump do not exist! Please clean and build the project first!\n"
		exit 1
	fi
	printf "Object dumping $TEST_BINARY to $TEST_DISASM\n"
	objdump -StTwx "$TEST_BINARY">"$TEST_DISASM"
fi

"$PROJECT_PATH/bin/driver/CodeParser_driver"
