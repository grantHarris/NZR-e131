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
#include <boost/thread/condition_variable.hpp>

using namespace boost::log;
namespace logging = boost::log;

struct Pixel {
     uint32_t r;
     uint32_t g;
     uint32_t b;
 };

 class LEDStrip {
    protected:
        bool *running;
        std::queue<std::vector<Pixel>> the_queue;
    public:
        mutable boost::mutex the_mutex;
        boost::condition_variable the_condition_variable;
        boost::thread* thread;
        
        LEDStrip(bool *t_running) : running(t_running){
            BOOST_LOG_TRIVIAL(info) << "Led strip constructor";
            thread = new boost::thread(boost::bind(&LEDStrip::wait_and_pop, this));
            BOOST_LOG_TRIVIAL(info) << "Led strip thread created";
        }

        virtual void write_pixels_to_strip(std::vector<Pixel>& t_pixels) = 0;
         
        void push(std::vector<Pixel> const& t_pixels)
        {
            BOOST_LOG_TRIVIAL(info) << "Push executed";
            boost::unique_lock<boost::mutex> lock(the_mutex);
            the_queue.push(t_pixels);
            lock.unlock();
            the_condition_variable.notify_one();
        }

        // bool empty() const
        // {
        //     boost::mutex::scoped_lock lock(the_mutex);
        //     return the_queue.empty();
        // }

        void wait_and_pop()
        {
            boost::unique_lock<boost::mutex> lock(the_mutex);
            BOOST_LOG_TRIVIAL(info) << "wait and pop";
            while(*running == true){
                BOOST_LOG_TRIVIAL(info) << "foo";
                while(the_queue.empty())
                {
                    the_condition_variable.wait(lock);
                }
                
                //this->write_pixels_to_strip(the_queue.front());
                //the_queue.pop();
            }
        }
 };

 #endif /* __LEDSTRIP_H__ */

