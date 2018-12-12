
#include <vector>
#include <iostream>
#include <iomanip>

#include <timeitmagic.h>


const int BLOCK_SIZE=16;


struct Worker{
   // there is no overhead in using std::vector
   // but just in case...
   unsigned int *mem;
   int n;  
   unsigned int result;
   void operator()(){
        for(int i=0;i<n;i+=BLOCK_SIZE){           
             result+=mem[i];
        }
   }

   Worker(std::vector<unsigned int> &mem_):
       mem(mem_.data()),n(static_cast<int>(mem_.size())), result(1.0)
   {}
};

double PREVENT_OPTIMIZATION=0.0;
const int ITER=200;
const int TRIES=5;

double get_size_in_kB(int SIZE){
   return SIZE*sizeof(int)/(1024.0);
}

double get_speed_in_GB_per_sec(int SIZE){
   std::vector<unsigned int> vals(SIZE, 42);
   Worker worker(vals);
   double time=timeitmagic::timeit(worker, TRIES, ITER).time;
   PREVENT_OPTIMIZATION+=worker.result;
   return get_size_in_kB(SIZE)/(1024*1024)/time;
}



int main(){
 
   
   int size=BLOCK_SIZE*16;
   std::cout<<"size(kB),bandwidth(GB/s)\n";
   while(size<5e7){
       std::cout<<get_size_in_kB(size)<<","<<get_speed_in_GB_per_sec(size)<<"\n";
       size=(static_cast<int>(size*1.2)/BLOCK_SIZE)*BLOCK_SIZE;
   }

   //ensure that nothing is optimized away:
   std::cerr<<"Sum: "<<PREVENT_OPTIMIZATION<<"\n";
}
