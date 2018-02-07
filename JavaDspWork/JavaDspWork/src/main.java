import java.io.IOException;

public class main {

	public static void main(String[] args) throws IOException {
		
		double [] audiobuffer = {0};
		double [] autocorrResult = {0};
		double fundamental = 0;
		int startFrame = 0, endFrame , bufferSize = 1024, count=0;
		
		ReadAudioFile raf = new ReadAudioFile();
		AutoCorrelation autocorr = new AutoCorrelation ();
		NoiseRemover nr = new NoiseRemover();
		
		endFrame = bufferSize;
		
		for (int i =0; i <Math.floorDiv(raf.wavFile.getNumFrames(), 1024); i ++)
		{
			audiobuffer = raf.getAudio(startFrame, endFrame, bufferSize);			
			nr.removeNoise(audiobuffer);
			System.out.println(nr.getSummation());
			
			if (nr.getSummation() <4.5)
			{
				System.out.println("noise decteted at chunk: " + (i+1) );
			}
			
			else 
			{			
				autocorrResult = autocorr.autoCorrelate(audiobuffer);
				System.out.println(autocorr.getFundamental(autocorrResult) + "  " + i+1);
			}
			startFrame = startFrame + bufferSize;
			endFrame = endFrame + bufferSize;
			
		}
				
		
		
		raf.wavFile.close();
		
		
		//autocorrResult = autocorr.autoCorrelate(audiobuffer);
		//fundamental = autocorr.getFundamental(autocorrResult);
		
		
		
		
		System.out.println("done!");
	}

}
