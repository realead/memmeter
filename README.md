# memmeter

experiments with memory


## band width

My machine can do about 10^9 double-multiplications per second,  that means at most 8GB/s per single processor. But data is fetched in 64bytes, so steping over 7 doubles and utilizing only 8 bytes we get to 64 GB/s. A integer addition can be done in 0.35 seconds thus leading to maximal band width of 182 GB/s.


Running `sh run_test.sh band_width` we get the following results:


![1](results/output_test_band_width.png)

Results per single thread:

    Memory type     Size         Speed (GB/s)
    L1 Cache:       32kB          140            *there are not really 64 fetched, 
    L2 Cache:      128(256)kB      80            *For whatever reason the second 128k pof 256kB L2-cachel are not that easily utilized
    L3 Cache:        4MB           30
    RAM:             XXX           13


There are some things worth mentioning:

  * a performance drop of the L1 cache for sizes around 2KB
  * drop of speed for L1->L2 is more abrupt then L2->L3, it seems as if L1 were not shared between threads, but L2 is (at least to some degree)
  * L3 seems to be shared among all processors/threads
