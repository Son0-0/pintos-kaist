!/bin/bash

source ./activate
make clean
cd ./vm
make
cd ./build

# pintos --gdb   --fs-disk=10 -p tests/userprog/exec-once:exec-once -p tests/userprog/child-simple:child-simple --swap-disk=4 -- -q   -f run exec-once
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/close-bad-fd:close-bad-fd --swap-disk=4 -- -q   -f run close-bad-fd 
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/exec-once:exec-once -p tests/userprog/child-simple:child-simple --swap-disk=4 -- -q   -f run exec-once
# 승우꺼에서 마킹 해보기