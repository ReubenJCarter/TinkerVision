#pragma once

#include "TinkerVision_export.h"

#include <functional> 

#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <vector>
#include <stdint.h>
#include <atomic>

#include <iostream>

namespace TnkrVis
{
	
class TINKERVISION_EXPORT ParallelFor
{
    private: 
        std::vector<std::thread> workers; 

        std::mutex mutex;
        std::condition_variable conditionVar;
        
        bool kill; 

        std::queue<std::function<void()>> funcQueue;

		ParallelFor(int workerCount=std::thread::hardware_concurrency()); 
        ~ParallelFor(); 
		
	public:
		ParallelFor(ParallelFor const&) = delete;
        void operator=(ParallelFor const&) = delete;
		
		static ParallelFor& GetInstance()
        {
            static ParallelFor instance;
            return instance;
        }
		
        inline void Run(int itemCount, std::function<void(int)> innerForLoop)
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
        
        inline void Run(int itemCountX, int itemCountY, std::function<void(int, int)> innerForLoop)
        {
            int itemCount = itemCountX * itemCountY; 
            int itemsPerThread = itemCount / workers.size(); 
            int leftOvers = itemCount % workers.size(); 

            std::atomic<int> todoCount = 0;

            int startInx = 0; 
            for(int t = 0; t < workers.size(); t++)
            {
                
                int len = itemsPerThread + ((t < leftOvers) ? 1 : 0);

                if(len == 0)
                    break;

                {
                    std::unique_lock<std::mutex> lock(mutex);
                    todoCount++; 
                    funcQueue.push([this, len, startInx, innerForLoop, itemCountX, &todoCount]()
                    {
                        for(int i = startInx; i < startInx + len; i++)
                        {
                            int X = i % itemCountX;
                            int Y = i / itemCountX;
                            innerForLoop(X, Y); 
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
	
}