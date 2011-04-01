#ifndef RT_SIMPLETIMER_H
#define RT_SIMPLETIMER_H

#include <sys/time.h>
#include <unistd.h>

namespace rt {

class SimpleTimer
{
public:
	SimpleTimer()
	{
		gettimeofday(&begin_, NULL);
	}

	double elapsed()
	{
		gettimeofday(&end_, NULL);
		timersub(&end_, &begin_, &elapsed_);
		return elapsed_.tv_sec + (elapsed_.tv_usec / (1e6));
	}

private:
	struct timeval begin_;
	struct timeval end_;
	struct timeval elapsed_;
};

} // namespace rt

#endif
