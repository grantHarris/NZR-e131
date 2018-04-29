#ifndef __LEDSTRIP_H__
#define __LEDSTRIP_H__

#include <stdint.h>
#include <unistd.h>
#include <mutex>
#include <err.h>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include "yaml-cpp/yaml.h"

using namespace boost::log;
namespace logging = boost::log;

struct Pixel {
     uint32_t r;
     uint32_t g;
     uint32_t b;
 };

 class LEDStrip {
 	public:
 		LEDStrip(bool *running) : running(t_running){
 			thread = new boost::thread([=] { this->wait_and_pop(); });
 		}

 		~LEDStrip(){
 			thread.join();
 		}

        virtual void write_pixels_to_strip(std::vector<Pixel> t_pixels) = 0;
         
        void push(std::vector<Pixel> const& t_pixels)
        {
        	boost::mutex::scoped_lock lock(the_mutex);
	        the_queue.push(t_pixels);
	        lock.unlock();
	        the_condition_variable.notify_one();
	    }

	    bool empty() const
	    {
	        boost::mutex::scoped_lock lock(the_mutex);
	        return the_queue.empty();
	    }

	    void wait_and_pop()
	    {
	    	while(*running == true){
		        boost::mutex::scoped_lock lock(the_mutex);
		        while(the_queue.empty())
		        {
		            the_condition_variable.wait(lock);
		        }
		        
		        this->write_pixels_to_strip(the_queue.front());
		        the_queue.pop();
	    	}
	    }
    private:
    	std::queue<std::vector<Pixel>> the_queue;
    	mutable boost::mutex the_mutex;
    	boost::condition_variable the_condition_variable;
    	boost::thread thread;
 };

 #endif /* __LEDSTRIP_H__ */

