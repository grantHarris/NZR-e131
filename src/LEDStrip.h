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
        std::queue<std::vector<Pixel>> frame_queue;
    public:
        mutable boost::mutex frame_mutex;
        boost::condition_variable wait_for_frame;
        boost::thread* thread;
        
        LEDStrip(bool *t_running) : running(t_running)
        {
            BOOST_LOG_TRIVIAL(debug) << "Led strip constructor";
            thread = new boost::thread(boost::bind(&LEDStrip::pop_and_display_frame, this));
        }

        virtual void write_pixels_to_strip(std::vector<Pixel>& t_pixels) = 0;
         
        void push_frame(std::vector<Pixel> const& t_pixels)
        {
            boost::unique_lock<boost::mutex> lock(frame_mutex);
            frame_queue.push(t_pixels);
            lock.unlock();
            wait_for_frame.notify_one();
        }

        void pop_and_display_frame()
        {
            boost::unique_lock<boost::mutex> lock(frame_mutex);
            BOOST_LOG_TRIVIAL(info) << "wait and pop";
            while(*running == true)
            {
                while(frame_queue.empty())
                {
                    if(*running == true){
                        wait_for_frame.wait(lock);
                    }else{
                        lock.unlock();
                        break;
                    }
                }
                this->write_pixels_to_strip(frame_queue.front());
                frame_queue.pop();
            }
        }
 };

 #endif /* __LEDSTRIP_H__ */

