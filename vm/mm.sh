#! /bin/bash

source ../activate
make
cd build

# Project 3 - VM - Page merge
pintos -v -k -T 600 -m 20   --fs-disk=10 -p tests/vm/page-merge-seq:page-merge-seq -p tests/vm/child-sort:child-sort --swap-disk=4 -- -q   -f run page-merge-seq < /dev/null 2> tests/vm/page-merge-seq.errors > tests/vm/page-merge-seq.output
perl -I../.. ../../tests/vm/page-merge-seq.ck tests/vm/page-merge-seq tests/vm/page-merge-seq.result
pintos -v -k -T 600 -m 20   --fs-disk=10 -p tests/vm/page-merge-par:page-merge-par -p tests/vm/child-sort:child-sort --swap-disk=10 -- -q   -f run page-merge-par < /dev/null 2> tests/vm/page-merge-par.errors > tests/vm/page-merge-par.output
perl -I../.. ../../tests/vm/page-merge-par.ck tests/vm/page-merge-par tests/vm/page-merge-par.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/page-merge-stk:page-merge-stk -p tests/vm/child-qsort:child-qsort --swap-disk=10 -- -q   -f run page-merge-stk < /dev/null 2> tests/vm/page-merge-stk.errors > tests/vm/page-merge-stk.output
perl -I../.. ../../tests/vm/page-merge-stk.ck tests/vm/page-merge-stk tests/vm/page-merge-stk.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/page-merge-mm:page-merge-mm -p tests/vm/child-qsort-mm:child-qsort-mm --swap-disk=10 -- -q   -f run page-merge-mm < /dev/null 2> tests/vm/page-merge-mm.errors > tests/vm/page-merge-mm.output
perl -I../.. ../../tests/vm/page-merge-mm.ck tests/vm/page-merge-mm tests/vm/page-merge-mm.result
pintos -v -k -T 300 -m 20   --fs-disk=10 -p tests/vm/page-linear:page-linear --swap-disk=4 -- -q   -f run page-linear < /dev/null 2> tests/vm/page-linear.errors > tests/vm/page-linear.output
perl -I../.. ../../tests/vm/page-linear.ck tests/vm/page-linear tests/vm/page-linear.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/page-parallel:page-parallel -p tests/vm/child-linear:child-linear --swap-disk=4 -- -q   -f run page-parallel < /dev/null 2> tests/vm/page-parallel.errors > tests/vm/page-parallel.output
perl -I../.. ../../tests/vm/page-parallel.ck tests/vm/page-parallel tests/vm/page-parallel.result
pintos -v -k -T 600 -m 20   --fs-disk=10 -p tests/vm/page-shuffle:page-shuffle --swap-disk=4 -- -q   -f run page-shuffle < /dev/null 2> tests/vm/page-shuffle.errors > tests/vm/page-shuffle.output
perl -I../.. ../../tests/vm/page-shuffle.ck tests/vm/page-shuffle tests/vm/page-shuffle.result

# Project 3 - VM - Swap
pintos -v -k -T 180 -m 8   --fs-disk=10 -p tests/vm/swap-file:swap-file -p ../../tests/vm/large.txt:large.txt --swap-disk=10 -- -q   -f run swap-file < /dev/null 2> tests/vm/swap-file.errors > tests/vm/swap-file.output
perl -I../.. ../../tests/vm/swap-file.ck tests/vm/swap-file tests/vm/swap-file.result
pintos -v -k -T 180 -m 10   --fs-disk=10 -p tests/vm/swap-anon:swap-anon --swap-disk=30 -- -q   -f run swap-anon < /dev/null 2> tests/vm/swap-anon.errors > tests/vm/swap-anon.output
perl -I../.. ../../tests/vm/swap-anon.ck tests/vm/swap-anon tests/vm/swap-anon.result
pintos -v -k -T 180 -m 10   --fs-disk=10 -p tests/vm/swap-iter:swap-iter -p ../../tests/vm/large.txt:large.txt --swap-disk=50 -- -q   -f run swap-iter < /dev/null 2> tests/vm/swap-iter.errors > tests/vm/swap-iter.output
perl -I../.. ../../tests/vm/swap-iter.ck tests/vm/swap-iter tests/vm/swap-iter.result
pintos -v -k -T 600 -m 40   --fs-disk=10 -p tests/vm/swap-fork:swap-fork -p tests/vm/child-swap:child-swap --swap-disk=200 -- -q   -f run swap-fork < /dev/null 2> tests/vm/swap-fork.errors > tests/vm/swap-fork.output
perl -I../.. ../../tests/vm/swap-fork.ck tests/vm/swap-fork tests/vm/swap-fork.result

# # Project 3 - VM - mmap
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-read:mmap-read -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run mmap-read < /dev/null 2> tests/vm/mmap-read.errors > tests/vm/mmap-read.output
# perl -I../.. ../../tests/vm/mmap-read.ck tests/vm/mmap-read tests/vm/mmap-read.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-close:mmap-close -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run mmap-close < /dev/null 2> tests/vm/mmap-close.errors > tests/vm/mmap-close.output
# perl -I../.. ../../tests/vm/mmap-close.ck tests/vm/mmap-close tests/vm/mmap-close.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-unmap:mmap-unmap -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run mmap-unmap < /dev/null 2> tests/vm/mmap-unmap.errors > tests/vm/mmap-unmap.output
# perl -I../.. ../../tests/vm/mmap-unmap.ck tests/vm/mmap-unmap tests/vm/mmap-unmap.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-overlap:mmap-overlap -p tests/vm/zeros:zeros --swap-disk=4 -- -q   -f run mmap-overlap < /dev/null 2> tests/vm/mmap-overlap.errors > tests/vm/mmap-overlap.output
# perl -I../.. ../../tests/vm/mmap-overlap.ck tests/vm/mmap-overlap tests/vm/mmap-overlap.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-twice:mmap-twice -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run mmap-twice < /dev/null 2> tests/vm/mmap-twice.errors > tests/vm/mmap-twice.output
# perl -I../.. ../../tests/vm/mmap-twice.ck tests/vm/mmap-twice tests/vm/mmap-twice.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-write:mmap-write --swap-disk=4 -- -q   -f run mmap-write < /dev/null 2> tests/vm/mmap-write.errors > tests/vm/mmap-write.output
# perl -I../.. ../../tests/vm/mmap-write.ck tests/vm/mmap-write tests/vm/mmap-write.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-ro:mmap-ro -p ../../tests/vm/large.txt:large.txt --swap-disk=4 -- -q   -f run mmap-ro < /dev/null 2> tests/vm/mmap-ro.errors > tests/vm/mmap-ro.output
# perl -I../.. ../../tests/vm/mmap-ro.ck tests/vm/mmap-ro tests/vm/mmap-ro.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-exit:mmap-exit -p tests/vm/child-mm-wrt:child-mm-wrt --swap-disk=4 -- -q   -f run mmap-exit < /dev/null 2> tests/vm/mmap-exit.errors > tests/vm/mmap-exit.output
# perl -I../.. ../../tests/vm/mmap-exit.ck tests/vm/mmap-exit tests/vm/mmap-exit.result
# pintos -v -k -T 600 -m 20   --fs-disk=10 -p tests/vm/mmap-shuffle:mmap-shuffle --swap-disk=4 -- -q   -f run mmap-shuffle < /dev/null 2> tests/vm/mmap-shuffle.errors > tests/vm/mmap-shuffle.output
# perl -I../.. ../../tests/vm/mmap-shuffle.ck tests/vm/mmap-shuffle tests/vm/mmap-shuffle.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-bad-fd:mmap-bad-fd --swap-disk=4 -- -q   -f run mmap-bad-fd < /dev/null 2> tests/vm/mmap-bad-fd.errors > tests/vm/mmap-bad-fd.output
# perl -I../.. ../../tests/vm/mmap-bad-fd.ck tests/vm/mmap-bad-fd tests/vm/mmap-bad-fd.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-clean:mmap-clean -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run mmap-clean < /dev/null 2> tests/vm/mmap-clean.errors > tests/vm/mmap-clean.output
# perl -I../.. ../../tests/vm/mmap-clean.ck tests/vm/mmap-clean tests/vm/mmap-clean.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-inherit:mmap-inherit -p ../../tests/vm/sample.txt:sample.txt -p tests/vm/child-inherit:child-inherit --swap-disk=4 -- -q   -f run mmap-inherit < /dev/null 2> tests/vm/mmap-inherit.errors > tests/vm/mmap-inherit.output
# perl -I../.. ../../tests/vm/mmap-inherit.ck tests/vm/mmap-inherit tests/vm/mmap-inherit.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-misalign:mmap-misalign -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run mmap-misalign < /dev/null 2> tests/vm/mmap-misalign.errors > tests/vm/mmap-misalign.output
# perl -I../.. ../../tests/vm/mmap-misalign.ck tests/vm/mmap-misalign tests/vm/mmap-misalign.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-null:mmap-null -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run mmap-null < /dev/null 2> tests/vm/mmap-null.errors > tests/vm/mmap-null.output
# perl -I../.. ../../tests/vm/mmap-null.ck tests/vm/mmap-null tests/vm/mmap-null.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-over-code:mmap-over-code -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run mmap-over-code < /dev/null 2> tests/vm/mmap-over-code.errors > tests/vm/mmap-over-code.output
# perl -I../.. ../../tests/vm/mmap-over-code.ck tests/vm/mmap-over-code tests/vm/mmap-over-code.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-over-data:mmap-over-data -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run mmap-over-data < /dev/null 2> tests/vm/mmap-over-data.errors > tests/vm/mmap-over-data.output
# perl -I../.. ../../tests/vm/mmap-over-data.ck tests/vm/mmap-over-data tests/vm/mmap-over-data.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-over-stk:mmap-over-stk -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run mmap-over-stk < /dev/null 2> tests/vm/mmap-over-stk.errors > tests/vm/mmap-over-stk.output
# perl -I../.. ../../tests/vm/mmap-over-stk.ck tests/vm/mmap-over-stk tests/vm/mmap-over-stk.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-remove:mmap-remove -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run mmap-remove < /dev/null 2> tests/vm/mmap-remove.errors > tests/vm/mmap-remove.output
# perl -I../.. ../../tests/vm/mmap-remove.ck tests/vm/mmap-remove tests/vm/mmap-remove.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-zero:mmap-zero --swap-disk=4 -- -q   -f run mmap-zero < /dev/null 2> tests/vm/mmap-zero.errors > tests/vm/mmap-zero.output
# perl -I../.. ../../tests/vm/mmap-zero.ck tests/vm/mmap-zero tests/vm/mmap-zero.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-bad-fd2:mmap-bad-fd2 --swap-disk=4 -- -q   -f run mmap-bad-fd2 < /dev/null 2> tests/vm/mmap-bad-fd2.errors > tests/vm/mmap-bad-fd2.output
# perl -I../.. ../../tests/vm/mmap-bad-fd2.ck tests/vm/mmap-bad-fd2 tests/vm/mmap-bad-fd2.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-bad-fd3:mmap-bad-fd3 --swap-disk=4 -- -q   -f run mmap-bad-fd3 < /dev/null 2> tests/vm/mmap-bad-fd3.errors > tests/vm/mmap-bad-fd3.output
# perl -I../.. ../../tests/vm/mmap-bad-fd3.ck tests/vm/mmap-bad-fd3 tests/vm/mmap-bad-fd3.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-zero-len:mmap-zero-len --swap-disk=4 -- -q   -f run mmap-zero-len < /dev/null 2> tests/vm/mmap-zero-len.errors > tests/vm/mmap-zero-len.output
# perl -I../.. ../../tests/vm/mmap-zero-len.ck tests/vm/mmap-zero-len tests/vm/mmap-zero-len.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-off:mmap-off -p ../../tests/vm/large.txt:large.txt --swap-disk=4 -- -q   -f run mmap-off < /dev/null 2> tests/vm/mmap-off.errors > tests/vm/mmap-off.output
# perl -I../.. ../../tests/vm/mmap-off.ck tests/vm/mmap-off tests/vm/mmap-off.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-bad-off:mmap-bad-off -p ../../tests/vm/large.txt:large.txt --swap-disk=4 -- -q   -f run mmap-bad-off < /dev/null 2> tests/vm/mmap-bad-off.errors > tests/vm/mmap-bad-off.output
# perl -I../.. ../../tests/vm/mmap-bad-off.ck tests/vm/mmap-bad-off tests/vm/mmap-bad-off.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/mmap-kernel:mmap-kernel -p ../../tests/vm/sample.txt:sample.txt --swap-disk=4 -- -q   -f run mmap-kernel < /dev/null 2> tests/vm/mmap-kernel.errors > tests/vm/mmap-kernel.output
# perl -I../.. ../../tests/vm/mmap-kernel.ck tests/vm/mmap-kernel tests/vm/mmap-kernel.result
# pintos -v -k -T 600 -m 20   --fs-disk=10 -p tests/vm/lazy-file:lazy-file -p ../../tests/vm/sample.txt:sample.txt -p ../../tests/vm/small.txt:small.txt --swap-disk=4 -- -q   -f run lazy-file < /dev/null 2> tests/vm/lazy-file.errors > tests/vm/lazy-file.output
# perl -I../.. ../../tests/vm/lazy-file.ck tests/vm/lazy-file tests/vm/lazy-file.result
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/vm/lazy-anon:lazy-anon --swap-disk=4 -- -q   -f run lazy-anon < /dev/null 2> tests/vm/lazy-anon.errors > tests/vm/lazy-anon.output
# perl -I../.. ../../tests/vm/lazy-anon