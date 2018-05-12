
#include <vector>
#include <iostream>
#include <iomanip>

#include <timeitmagic.h>


const int BLOCK_SIZE=16;

unsigned int next(unsigned int current, unsigned int SIZE){
   return (current*10001+327)%(SIZE-1);
}


#include <iostream>
#include <random>


struct Worker{
   unsigned int *mem;
   unsigned int n; 
   unsigned int steps;
   int result;
   void operator()(){
        int index=0;
        for(unsigned int i=0;i<steps;i++){           
             index=mem[index];
        }
        result=index;
   }

   Worker(std::vector<unsigned int> &mem_):
       mem(mem_.data()),n(static_cast<int>(mem_.size())), result(1.0)
   {}
};

double PREVENT_OPTIMIZATION=0.0;
const int ITER=4;
const int TRIES=3;

double get_size_in_kB(int SIZE){
   return SIZE*sizeof(int)/(1024.0);
}

std::vector<unsigned int> make_perm(int SIZE){
   std::random_device rd;
   std::mt19937 gen(rd());
   std::vector<unsigned int> perm(SIZE);
   for(int i=0;i<SIZE;i++)
     perm[i]=i;
   //Fisher-Yates
   for(int k=0;k<SIZE;k++){
      std::uniform_int_distribution<> dis(k, SIZE-1);
      int for_swap=dis(gen);
      std::swap(perm[k], perm[for_swap]);
   }
   //inverse:
   std::vector<unsigned int> iperm(SIZE);
   for(int i=0;i<SIZE;i++){
      iperm[perm[i]]=i;
   }

   std::vector<unsigned int> result(SIZE);
   for(int i=0;i<SIZE;i++){
      unsigned int ii=(perm[i]+1)%SIZE;
      result[i]=iperm[ii];
   } 
   return result;
}

double get_latency_in_sec(int SIZE){

   std::vector<unsigned int> vals=make_perm(SIZE);

        
   
   Worker worker(vals);
   worker.steps=worker.n*1;
   double time=timeitmagic::timeit(worker, TRIES, ITER).time;
   PREVENT_OPTIMIZATION+=worker.result;
   return time/worker.steps;
}




int main(){
 
   
   int size=BLOCK_SIZE*16;
   std::cout<<"size(kB),latency(s)\n";
   while(size<5e7){
       std::cout<<get_size_in_kB(size)<<","<<get_latency_in_sec(size)<<"\n";
       size=(static_cast<int>(size*1.2)/BLOCK_SIZE)*BLOCK_SIZE;
   }

   //ensure that nothing is optimized away:
   std::cerr<<"Sum: "<<PREVENT_OPTIMIZATION<<"\n";
}
