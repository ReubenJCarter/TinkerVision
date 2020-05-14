//run like   java -cp .\* JavaTest aruco.png

import TnkrVisJava.TnkrVis.Color;
import TnkrVisJava.TnkrVis.*;
import TnkrVisJava.TnkrVis.IO.*;
import TnkrVisJava.TnkrVis.Process.*;
import TnkrVisJava.TnkrVis.CompositeProcess.*;

public class JavaTest {

    static {
        System.loadLibrary("TinkerVision_Java") ;
    }

    public static void main(String[] args) {
        // Prints "Hello, World" to the terminal window.
        
        Context context = new Context(); 
	    context.MakeCurrent(); 

        System.out.println("creating color" );
        Color c = new Color(); 
        c.setR(1.0f); 
        System.out.println("Color:c.R=" + Float.toString(c.getR()));

        Image image1= new Image(); 
		Image image2 = new Image();
		Image image3= new Image();

		ImageGPU imageGPU1 = new ImageGPU();
		ImageGPU imageGPU2 = new ImageGPU();
		ImageGPU imageGPU3 = new ImageGPU();
		ImageGPU imageGPU4 = new ImageGPU();

        System.out.println("args[0]" + args[0]);
        
        ImageFile.Read(args[0], image1); 

        ImageFile.Write("JAVATest1.png", image1); 

        imageGPU1.Copy(image1); 


        //Brightness Contrast
		System.out.println( "Brightness Contrast\n");
		BrightnessContrast brightnessContrast = new BrightnessContrast(); 
		brightnessContrast.SetBrightness(-1); 
		brightnessContrast.SetContrast(3); 
		brightnessContrast.Run(imageGPU1, imageGPU2);
		image2.Copy(imageGPU2);
		ImageFile.Write("JAVAimage1_1Test.png", image2);
		brightnessContrast.Run(image1, image2); 
		ImageFile.Write("JAVAimage1_2Test.png", image2);

        //Canny Edges
		System.out.println("Canny Edges\n");
		CannyEdgeDetect canny = new CannyEdgeDetect(); 
		canny.SetBlurSigma(0.5f); 
		canny.SetLowEdgeThreshold(0.1f);
		canny.SetHighEdgeThreshold(0.2f);

		canny.Run(imageGPU1, image2); 
		ImageFile.Write("JAVAimage12_1Test.png", image2);
    }

}