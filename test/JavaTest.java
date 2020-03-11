import VisiJava.Visi.Color;
import VisiJava.Visi.*;
import VisiJava.Visi.IO.*;
//run like   java -cp .\* JavaTest 
public class JavaTest {

    static {
        System.loadLibrary("Visi_Java") ;
    }

    public static void main(String[] args) {
        // Prints "Hello, World" to the terminal window.
        
        Color c = new Color(); 
        c.setR(1.0f); 
        System.out.println("Color:c.R=" + Float.toString(c.getR()));



        System.out.println("args[0]" + args[0]);
        Image image0 = new Image();
        
        ImageFile.Read(args[0], image0); 

        ImageFile.Write("JavaTest1.png", image0); 
    }

}