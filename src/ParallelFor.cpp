#include "ParallelFor.h"

#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <vector>
#include <stdint.h>
#include <atomic>

namespace Visi
{

class ParallelFor::Internal
{
    private:
        std::vector<std::thread> workers; 

        std::mutex mutex;
        std::condition_variable conditionVar;
        
        bool kill; 

        std::queue<std::function<void()>> funcQueue;

    public:
        Internal(int workerCount=std::thread::hardware_concurrency())
        {
            kill = false; 

            for(int i = 0; i < workerCount; i++)
            {
                workers.push_back(std::thread([this](int workerInx)
                {
                
                    while(true)
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        conditionVar.wait(lock, [this]{ return kill || !funcQueue.empty(); });

                        if(kill)
                        {
                            return; 
                        }

                        std::function<void()> func = funcQueue.front();
                        funcQueue.pop();

                        mutex.unlock();

                        func(); 

                        
                    }
                }, i)); 
            }
        }

        ~Internal()
        {
            kill = true; 
        }
        
        void Run(int itemCount, std::function<void(int)> innerForLoop)
        {
            int itemsPerThread = itemCount / workers.size(); 
            int leftOvers = itemCount % workers.size(); 

            std::atomic<int> todoCount = 0;

            int startInx = 0; 
            for(int t = 0; t < workers.size(); t++)
            {
                
                int len = itemsPerThread + (t < leftOvers) ? 1 : 0;

                if(len == 0)
                    break;

                {
                    std::unique_lock<std::mutex> lock(mutex);
                    todoCount++; 
                    funcQueue.push([this, len, startInx, innerForLoop, &todoCount]()
                    {
                        for(int i = startInx; i < startInx + len; i++)
                        {
                            innerForLoop(i); 
                        }
                        todoCount--;
                    });
                }

                conditionVar.notify_one();

                startInx +=  len;
            }

            //wait for all threads to finish (could do this with conditional...)
            while(todoCount != 0)
            {
                
            }

        }
};


ParallelFor::ParallelFor()
{
    internal = new Internal(); 
}

ParallelFor::~ParallelFor()
{
    delete internal; 
}

void ParallelFor::Run(int itemCount, std::function<void(int)> innerForLoop)
{
    internal->Run(itemCount, innerForLoop); 
}

}