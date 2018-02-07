import java.io.*;

public class ReadAudioFile
{
	public WavFile wavFile;
	
	public ReadAudioFile()
	{
		try {
			 wavFile = WavFile.openWavFile(new File("C:\\Users\\Donnacha\\Desktop\\Final-Project\\GuitarNotes+TestSignals\\C_B_A_G_F#_E_fast.wav"));
		} catch (IOException | WavFileException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
	}
	public double [] getAudio (int startSample , int endSample , int bufferSize)
	{
		double[] buffer = {0};
		
		try
		{
			// Open the wav file specified as the first argument
			//WavFile wavFile = WavFile.openWavFile(new File("C:\\Users\\Donnacha\\Desktop\\Final-Project\\GuitarNotes+TestSignals\\C_B_A_G_F#_E_fast.wav"));

			// Display information about the wav file
			//wavFile.display();

			// Get the number of audio channels in the wav file
			//int numChannels = wavFile.getNumChannels();
           // long numOfFrames =  wavFile.getNumFrames();
          
			// Create a buffer of 100 frames
		    buffer = new double[bufferSize+startSample];

			int framesRead;
		
			do
			{	
				// Read frames into buffer
				framesRead = wavFile.readFrames(buffer, startSample, bufferSize);

			}
			while (framesRead <bufferSize);

			// Close the wavFile
			//wavFile.close();

			// Output the minimum and maximum value
		
		}
		catch (Exception e)
		{
			System.err.println(e);
		}
		return buffer;
	}
}
