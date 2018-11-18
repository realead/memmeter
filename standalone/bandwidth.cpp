
#include <vector>
#include <chrono>
#include <iostream>
#include <algorithm>


//returns minimal time needed for one execution in seconds:
template<typename Fun>
double timeit(Fun&& stmt, int repeat, int number)
{  
   std::vector<double> times;
   for(int i=0;i<repeat;i++){
       auto begin = std::chrono::high_resolution_clock::now();
       for(int i=0;i<number;i++){
          stmt();
       }
       auto end = std::chrono::high_resolution_clock::now();
       double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()/1e9/number;
       times.push_back(time);
   }
   return *std::min_element(times.begin(), times.end());
}


const int NITER=200;
const int NTRIES=5;
const size_t BLOCK_SIZE=16;


struct Worker{
   std::vector<int> &mem;
   size_t n;
   int result;
   void operator()(){
        for(size_t i=0;i<n;i+=BLOCK_SIZE){           
             result+=mem[i];
        }
   }

   Worker(std::vector<int> &mem_):
       mem(mem_), n(mem.size()), result(1)
   {}
};

double PREVENT_OPTIMIZATION=0.0;


double get_size_in_kB(int SIZE){
   return SIZE*sizeof(int)/(1024.0);
}

double get_speed_in_GB_per_sec(int SIZE){
   std::vector<int> vals(SIZE, 42);
   Worker worker(vals);
   double time=timeit(worker, NTRIES, NITER);
   PREVENT_OPTIMIZATION+=worker.result;
   return get_size_in_kB(SIZE)/(1024*1024)/time;
}


int main(){
  
   int size=BLOCK_SIZE*16;
   std::cout<<"size(kB),bandwidth(GB/s)\n";
   while(size<4e4){
       std::cout<<get_size_in_kB(size)<<","<<get_speed_in_GB_per_sec(size)<<"\n";
       size=(static_cast<int>(size*1.1)/BLOCK_SIZE)*BLOCK_SIZE;
   }

   //ensure that nothing is optimized away:
   std::cerr<<"Sum: "<<PREVENT_OPTIMIZATION<<"\n";
}


