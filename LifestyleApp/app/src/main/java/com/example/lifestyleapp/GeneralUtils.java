package com.example.lifestyleapp;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import java.io.ByteArrayOutputStream;

/**
 * Class that contains general utility functions
 */
public class GeneralUtils {

    /**
     * Converts a "sex" boolean to the String variant.
     * False = Female
     * True = Male
     */
    public static String sexToString(boolean sex){
        if(sex){
            return "Male";
        }
        else{
            return "Female";
        }
    }

    /**
     * Converts a number of inches into a String representation in feet and height
     * such as: 5' 11"
     */
    public static String inchesToHeight(int in){
        int inches = in % 12;
        int feet = in / 12;

        return feet + "\' " + inches + "\"";
    }


    public static String convertActivityLevel(double level){
        if(level == 1.53){
            return "Sedentary";
        }
        else if (level == 1.76){
            return "Moderate";
        }
        else if (level == 2.25){
            return "Active";
        }
        return "Unknown Activity Level";
    }

    public static double convertActivityLevel(String level){
        if(level.equals("Sedentary")){
            return 1.53;
        }
        else if (level.equals("Moderate")){
            return 1.76;
        }
        else if (level.equals("Active")){
            return 2.25;
        }
        return -1;
    }

    public static byte[] convertImage(Bitmap image){
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        image.compress(Bitmap.CompressFormat.PNG, 100, stream);
        return stream.toByteArray();
    }

    public static Bitmap convertImage(byte[] image){
        return BitmapFactory.decodeByteArray(image, 0, image.length);
    }
    

}
