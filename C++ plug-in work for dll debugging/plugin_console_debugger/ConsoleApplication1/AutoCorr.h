

#ifndef AUTO_CORR
#define AUTO_CORR


#include <vector>


class AutoCorr
{
private:
	float sum,maxPeak,peak,fund;
	long timeTaken, interations;
	std::vector<float> autoCorrOutput;


public:
	AutoCorr();
	~AutoCorr();

	std::vector<float> performAutoCorr(float [], int, const int dsf);
	float getFundamental(std::vector<float>, const int sf ,const int dsf);
	long getExecutionTime() { return timeTaken; }
	long getTotalIterationsTaken() { return interations; }

};

#endif // !AUTO_CORR



