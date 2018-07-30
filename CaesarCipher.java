/**
 * A program to search for to encrypt and decrypt lines of text. See
 * @author GitHub : rMaxiQp
 */
import java.io.*;

public class CaesarCipher {

	public static void main(String[] strings) {
		boolean valid= true;
		int shift = 0;
		BufferedReader buffer = new BufferedReader(new InputStreamReader(System.in));

		while(valid){ //If the shift is valid
			System.out.println("Please enter the shift value (between -25..-1 and 1..25)");

			try {
				shift = Integer.parseInt(buffer.readLine());
			} catch(Exception e) {
				e.printStackTrace();
				continue;
			}

			valid = false;
			if (999==shift || -999==shift) {
				System.out.println("Using position shift");
			}
			else if (shift!=0 && shift <= 25 && shift >= -25) {
				System.out.println("Using shift value of " + shift);
			}
			else {
				System.out.println(shift + " is not a valid shift value.");
				valid = true;
			}
		}//end of while loop

		String source;

		while(!valid){//Shift the string
			System.out.println("Please enter the source text (empty line to quit)");

			try {
				source = buffer.readLine();
			} catch(Exception e) {
				e.printStackTrace();
				continue;
			}

			if ( 0 == source.trim().length() )//empty line
				break;
			System.out.println("Source   : " + source);
			System.out.println("Processed: " + cipher(source.toUpperCase(), shift));
		}//end of while loop

		System.out.println("Bye.");
	}//end of main

	public static String cipher(String sUpper, int shift){
		String out = "";
		int count = shift;
		if (shift == 999 || shift == -999 )
			count = 0;
		for (int i = 0; i < sUpper.length(); i++){
			char temp = sUpper.charAt(i);
			int distance = sUpper.charAt(i) - 'A';
			if (temp>='A' && temp <='Z') { //Alphabet only
				if ((distance + count)%26 < 0) {
					temp = (char) ('A' + 26 + (distance + count)%26);
				} else {
					temp = (char) ('A' + (distance + count)%26);
				}
			}
			out = out + temp;
			if (shift == 999)
				count ++;
			if (shift == -999)
				count --;
		}
		return out;
	}//end of cipher

}//end of program
