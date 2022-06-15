#!/bin/bash

source ./activate
make clean
cd ./vm
make
cd ./build

# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-grow-stack:pt-grow-stack --swap-disk=4 -- -q   -f run pt-grow-stack 
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-grow-bad:pt-grow-bad --swap-disk=4 -- -q   -f run pt-grow-bad 
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-grow-stk-sc:pt-grow-stk-sc --swap-disk=4 -- -q   -f run pt-grow-stk-sc 
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-write-code:pt-write-code --swap-disk=4 -- -q   -f run pt-write-code 
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-write-code2:pt-write-code2 -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run pt-write-code2 
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-big-stk-obj:pt-big-stk-obj --swap-disk=4 -- -q   -f run pt-big-stk-obj 
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-bad-addr:pt-bad-addr --swap-disk=4 -- -q   -f run pt-bad-addr 
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-bad-read:pt-bad-read -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run pt-bad-read 

pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-grow-stack:pt-grow-stack --swap-disk=4 -- -q   -f run pt-grow-stack < /dev/null 2> tests/vm/pt-grow-stack.errors > tests/vm/pt-grow-stack.output
perl -I../.. ../../tests/vm/pt-grow-stack.ck tests/vm/pt-grow-stack tests/vm/pt-grow-stack.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-grow-bad:pt-grow-bad --swap-disk=4 -- -q   -f run pt-grow-bad < /dev/null 2> tests/vm/pt-grow-bad.errors > tests/vm/pt-grow-bad.output
perl -I../.. ../../tests/vm/pt-grow-bad.ck tests/vm/pt-grow-bad tests/vm/pt-grow-bad.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-grow-stk-sc:pt-grow-stk-sc --swap-disk=4 -- -q   -f run pt-grow-stk-sc < /dev/null 2> tests/vm/pt-grow-stk-sc.errors > tests/vm/pt-grow-stk-sc.output
perl -I../.. ../../tests/vm/pt-grow-stk-sc.ck tests/vm/pt-grow-stk-sc tests/vm/pt-grow-stk-sc.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-write-code:pt-write-code --swap-disk=4 -- -q   -f run pt-write-code < /dev/null 2> tests/vm/pt-write-code.errors > tests/vm/pt-write-code.output
perl -I../.. ../../tests/vm/pt-write-code.ck tests/vm/pt-write-code tests/vm/pt-write-code.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-write-code2:pt-write-code2 -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run pt-write-code2 < /dev/null 2> tests/vm/pt-write-code2.errors > tests/vm/pt-write-code2.output
perl -I../.. ../../tests/vm/pt-write-code2.ck tests/vm/pt-write-code2 tests/vm/pt-write-code2.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-big-stk-obj:pt-big-stk-obj --swap-disk=4 -- -q   -f run pt-big-stk-obj < /dev/null 2> tests/vm/pt-big-stk-obj.errors > tests/vm/pt-big-stk-obj.output
perl -I../.. ../../tests/vm/pt-big-stk-obj.ck tests/vm/pt-big-stk-obj tests/vm/pt-big-stk-obj.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-bad-addr:pt-bad-addr --swap-disk=4 -- -q   -f run pt-bad-addr < /dev/null 2> tests/vm/pt-bad-addr.errors > tests/vm/pt-bad-addr.output
perl -I../.. ../../tests/vm/pt-bad-addr.ck tests/vm/pt-bad-addr tests/vm/pt-bad-addr.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/pt-bad-read:pt-bad-read -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run pt-bad-read < /dev/null 2> tests/vm/pt-bad-read.errors > tests/vm/pt-bad-read.output
perl -I../.. ../../tests/vm/pt-bad-read.ck tests/vm/pt-bad-read tests/vm/pt-bad-read.result
