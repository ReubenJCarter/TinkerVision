#include "ParallelFor.h"

#include <chrono>

namespace Visi
{

ParallelFor::ParallelFor(int workerCount)
{
    kill = false; 

    for(int i = 0; i < workerCount; i++)
    {
        workers.push_back(std::thread([this](int workerInx)
        {
                        
            while(true)
            {
                std::function<void()> func;

                {
                    std::unique_lock<std::mutex> lock(mutex);
                    conditionVar.wait(lock, [this]{ return kill || !funcQueue.empty(); });

                    if(kill)
                    {
                        return; 
                    }

                    func = std::move(funcQueue.front());
                    funcQueue.pop();

                }

                func();             
            }
        }, i)); 
    }
}

ParallelFor::~ParallelFor()
{
    kill = true; 
    for(int i = 0; i < workers.size(); i++)
    {
        workers[i].join(); 
    }
}

}