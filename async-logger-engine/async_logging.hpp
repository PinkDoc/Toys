//
//  Async logger
//
#pragma once

#include <string.h>

#include <memory>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>

namespace async_logging {
    
	class buffer {
	private:
		std::vector<char> buffer_;
		std::size_t end_index_;

		enum {
			buffer_size_ = 1024 * 1024 * 4,
		};

	public:
		buffer(const buffer&) = delete;
		buffer& operator= (const buffer&) = delete;

		buffer();
        char* peek();
		std::size_t remain() const ;
		std::size_t size() const ;
		void append(const char* log, std::size_t len);
		void reset();
	};	

    class logging {
    private:
    	std::mutex 								mutex_;
    	std::condition_variable 				condition_;			
    	std::vector<std::unique_ptr<buffer>> 	buffers_;			// Write to file

    	std::unique_ptr<buffer> current_buf_;			 
    	std::unique_ptr<buffer> next_buf_;		

		bool stop_;	 

		std::thread log_thread_;
    public:
		logging(const logging&) = delete;
		logging& operator= (const logging&) = delete;

		logging();
		~logging();

        void append(const char* log, std::size_t len);
		void thread_loop();
		void run();
    };



    inline buffer::buffer():
    	buffer_(buffer_size_),
    	end_index_(0)
    {}

    inline char* buffer::peek() 
	{
    	return buffer_.data();
    }

    inline std::size_t buffer::remain() const
    {
    	return buffer_size_ - end_index_;
    }

    inline void buffer::append(const char* log, std::size_t len)
    {
        memcpy(buffer_.data() + end_index_ , log, len);
    	end_index_ += len;
    }

	inline std::size_t buffer::size() const 
	{
		return end_index_;
	}

	inline void buffer::reset()
	{
		end_index_ = 0;
	}
	
	inline logging::logging():
		current_buf_(new buffer()),
		next_buf_(new buffer()),
		stop_(false)
	{}

	inline logging::~logging()
	{
		{
			std::unique_lock<std::mutex> lock(mutex_);
			stop_ = true;
		}

		condition_.notify_all();
		log_thread_.join();
	}

    inline void logging::append(const char* log, std::size_t len)
    {
    	std::unique_lock<std::mutex> lock(mutex_);
    	if (current_buf_->remain() > len) {
    		current_buf_->append(log, len);
    	} else {
            buffers_.push_back(std::move(current_buf_));
			
			if (next_buf_ != nullptr)
			{
				current_buf_ = std::move(next_buf_);
			}

			current_buf_->append(log, len);
			condition_.notify_all();
    	}
    }

	inline void logging::thread_loop()
	{
		std::unique_ptr<buffer> buffer1(new buffer());
		std::unique_ptr<buffer> buffer2(new buffer());
		std::vector<std::unique_ptr<buffer>> bufferToWrite;
		while (!stop_)
		{
			{
				std::unique_lock<std::mutex> lock(mutex_);
				if (buffers_.empty())
					condition_.wait(lock);
				
				buffers_.push_back(std::move(current_buf_));
				current_buf_ = std::move(buffer1);
				bufferToWrite.swap(buffers_);

				if (!next_buf_)
				{
					next_buf_ = std::move(buffer2);
				}
				
			}	// End 

			for (const auto& i : bufferToWrite)
			{
				// Write to file
			}

			if (!buffer1) {
				buffer1 = std::move(bufferToWrite.back());
				bufferToWrite.pop_back();
				buffer1->reset();
			}

			if (!buffer2) {
				buffer2 = std::move(bufferToWrite.back());
				bufferToWrite.pop_back();
				buffer2->reset();
			}

			bufferToWrite.clear();
			// flush

		}

		//flush
	}

}  
