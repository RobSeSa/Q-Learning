import java.io.*;  
import java.util.Scanner;  
public class ReadCSVExample1  
{  
    public static void main(String[] args) throws Exception  
    {  
        //parsing a CSV file into Scanner class constructor  
        Scanner sc = new Scanner(new File("/Users/robertsato/Desktop/Graduate School/Winter 2021/CSE 210A - Programming Languages/Q-Learning/matrix_data.csv"));  
        sc.useDelimiter(",");   //sets the delimiter pattern  
        int count = 0;
        while (sc.hasNext())  //returns a boolean value  
        {  
            System.out.print(sc.next());  //find and returns the next complete token from this scanner  
            count++;
        }   
        sc.close();  //closes the scanner  
        System.out.println(count);
    }  
}  