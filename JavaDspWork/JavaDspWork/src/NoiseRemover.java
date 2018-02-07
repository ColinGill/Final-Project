
public class NoiseRemover {
	
	public double summation = 0;

	public NoiseRemover() {
		
		
	}
	
	public double [] removeNoise (double [] audioIn)
	{
		summation = 0;
		
		for (int i =0; i < audioIn.length; i ++)
		{
			summation = Math.abs(audioIn[i])+ summation;
		}
		
		
		return audioIn;		
	}
	
	public double getSummation()
	{
		return summation;
	}
	

}
