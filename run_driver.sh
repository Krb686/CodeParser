#!/bin/bash
PROJECT_PATH="/home/kevin/gitrepos/CodeParser"
export LD_LIBRARY_PATH="$PROJECT_PATH/bin/lib/"
objdump -StTwx "$PROJECT_PATH/bin/test/test_binary">"$PROJECT_PATH/bin/test/test_dump.disasm"
"$PROJECT_PATH/bin/driver/CodeParser_driver"
