/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.logger;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintStream;

/**
 *
 * @author agustin
 */
public class Logger 
{
    private static String filePath = System.getProperty("user.dir").replaceAll("\\\\", "/") + "/log.csv";
    private static boolean writeToFile = true;
    private static boolean writeToConsole = true;
    
    public static void setWriteToFile(boolean status)
    {
        writeToFile = status;
    }
    
    public static void setWriteToConsole(boolean status)
    {
        writeToConsole = status;
    } 
    
    public static void setFilePath(String path)
    {
        filePath = path;
    }
    
    private static String getMethodName(final int depth)
    {
      final StackTraceElement[] ste = Thread.currentThread().getStackTrace();
      return ste[ste.length - 1 - depth].getMethodName(); 
    }    
    
    private static String getClassName(final int depth)
    {
      final StackTraceElement[] ste = Thread.currentThread().getStackTrace();
      return ste[ste.length - 1 - depth].getClassName(); 
    }        
    
    private static int getLineNumber(final int depth)
    {
      final StackTraceElement[] ste = Thread.currentThread().getStackTrace();
      return ste[ste.length - 1 - depth].getLineNumber(); 
    }  
    
    public static void log(String message)
    {
        String output = getClassName(0) + ";" + getMethodName(0) + ";" + String.valueOf(getLineNumber(1)) + ";" + message;
        try
        {
            if(writeToFile)
            {
                PrintStream ps = new PrintStream(new File(filePath));            
                ps.println(output);
                ps.close();            
            }
            if(writeToConsole)
            {
                System.out.println(output);
            }
        }
        catch(Exception e)
        {
        
        }
    }
}
