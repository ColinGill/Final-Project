
public class AutoCorrelation {

	public AutoCorrelation() 
	{
		
	}

	public double[] autoCorrelate(double[] audioIn) 
	{
		
		double sum = 0;
		double[] output = new double[audioIn.length];
		int n = 0, counter = 0;
		long startTime = System.currentTimeMillis();
		
		
		for (n = 0; n < audioIn.length; n++) //shift sample to left after each inner loop
		{

			for (int i = n; i < audioIn.length; i = i+32) 
			{
				
				counter ++; 
				sum = sum + (audioIn[i] * audioIn[i - n]); //computes product of signal and n shifted copy

			}
			output[n] = sum;
			sum = 0; //reset for next output value
		}
		
		long stopTime = System.currentTimeMillis();
		long totalTime = stopTime-startTime;
		System.out.println("number of iterations: " + counter);
		
		return output;

	}

	public double getFundamental(double[] autoCorrResult) {
		
		double fund = 0, peak = 0, maxPeak = 0;

		for (int i = 1; i < autoCorrResult.length-1; i++) 
		{
			
			if ((autoCorrResult[i] > autoCorrResult[i + 1]) && (autoCorrResult[i] > autoCorrResult[i - 1])) 
			{

				if (autoCorrResult[i] > peak) 
				{
					peak = autoCorrResult[i];
					maxPeak = i;
				}
			}
		}

		fund = 1 / (maxPeak / 44100);

		return fund;

	}

}
