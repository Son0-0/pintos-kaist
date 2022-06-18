#!/bin/bash

source ./activate
make clean
cd ./vm
make
cd ./build


# pintos -v -k -T 600 -m 20   --fs-disk=10 -p tests/vm/page-merge-seq:page-merge-seq -p tests/vm/child-sort:child-sort --swap-disk=4 -- -q   -f run page-merge-seq 

# < /dev/null 2> tests/vm/page-merge-seq.errors > tests/vm/page-merge-seq.output
# perl -I../.. ../../tests/vm/page-merge-seq.ck tests/vm/page-merge-seq tests/vm/page-merge-seq.result

# pintos -v -k -T 600 -m 20   --fs-disk=10 -p tests/vm/page-merge-par:page-merge-par -p tests/vm/child-sort:child-sort --swap-disk=10 -- -q   -f run page-merge-par 

# < /dev/null 2> tests/vm/page-merge-par.errors > tests/vm/page-merge-par.output
# perl -I../.. ../../tests/vm/page-merge-par.ck tests/vm/page-merge-par tests/vm/page-merge-par.result

pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/page-merge-stk:page-merge-stk -p tests/vm/child-qsort:child-qsort --swap-disk=10 -- -q   -f run page-merge-stk 

# < /dev/null 2> tests/vm/page-merge-stk.errors > tests/vm/page-merge-stk.output
# perl -I../.. ../../tests/vm/page-merge-stk.ck tests/vm/page-merge-stk tests/vm/page-merge-stk.result

# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/page-merge-mm:page-merge-mm -p tests/vm/child-qsort-mm:child-qsort-mm --swap-disk=10 -- -q   -f run page-merge-mm 

# < /dev/null 2> tests/vm/page-merge-mm.errors > tests/vm/page-merge-mm.output
# perl -I../.. ../../tests/vm/page-merge-mm.ck tests/vm/page-merge-mm tests/vm/page-merge-mm.result

# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/page-parallel:page-parallel -p tests/vm/child-linear:child-linear --swap-disk=4 -- -q   -f run page-parallel 

# < /dev/null 2> tests/vm/page-parallel.errors > tests/vm/page-parallel.output
# perl -I../.. ../../tests/vm/page-parallel.ck tests/vm/page-parallel tests/vm/page-parallel.result


# pintos -v -k -T 180 -m 8   --fs-disk=10 -p tests/vm/swap-file:swap-file -p ../../tests/vm/large.txt:large.txt --swap-disk=10 -- -q   -f run swap-file