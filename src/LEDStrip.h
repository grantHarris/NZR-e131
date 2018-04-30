#ifndef __LEDSTRIP_H__
#define __LEDSTRIP_H__
#include <queue>
#include <vector>
#include <stdint.h>
#include <unistd.h>
#include <mutex>
#include <err.h>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
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
 		LEDStrip(bool *t_running) : running(t_running){
 			BOOST_LOG_TRIVIAL(info) << "Led strip constructor"
 			thread = new boost::thread(&LEDStrip::wait_and_pop, this);
 			BOOST_LOG_TRIVIAL(info) << "Led strip thread created"
 		}

 		~LEDStrip(){
 			thread->join();
 		}

        	virtual void write_pixels_to_strip(std::vector<Pixel>& t_pixels) = 0;
         
        	void push(std::vector<Pixel> const& t_pixels)
       	 	{
       	 		BOOST_LOG_TRIVIAL(info) << "Push executed"
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
	bool *running;
    	std::queue<std::vector<Pixel>> the_queue;
    	mutable boost::mutex the_mutex;
    	boost::condition_variable the_condition_variable;
    	boost::thread* thread;
 };

 #endif /* __LEDSTRIP_H__ */

