/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.lib.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import org.apache.tika.exception.TikaException;
import org.apache.tika.metadata.Metadata;
import org.apache.tika.parser.ParseContext;
import org.apache.tika.parser.Parser;
import org.apache.tika.parser.mp3.Mp3Parser;
import org.xml.sax.ContentHandler;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

/**
 *
 * @author agustin
 */
public class MP3TagExtractor 
{
    public static final String TITLE = "title";
    public static final String ARTIST = "xmpDM:artist";
    public static final String COMPOSER = "xmpDM:composer";
    public static final String GENRE = "xmpDM:genre";
    public static final String ALBUM = "xmpDM:album";
    
    public static Map<String, String> getTags(File file)
    {
        try 
        {
            InputStream input = new FileInputStream(file);
            ContentHandler handler = new DefaultHandler();
            Metadata metadata = new Metadata();
            Parser parser = new Mp3Parser();
            ParseContext parseCtx = new ParseContext();
            parser.parse(input, handler, metadata, parseCtx);
            input.close();

            // List all metadata
            String[] metadataNames = metadata.names();

            Map<String, String> tagMap = new HashMap<>();
            for(String name : metadataNames)
            {
                tagMap.put(name, metadata.get(name));
            }
            return tagMap;
        } 
        catch (FileNotFoundException e)
        {
            e.printStackTrace();
        } 
        catch (IOException e) 
        {
            e.printStackTrace();
        } 
        catch (SAXException e) 
        {
            e.printStackTrace();
        } 
        catch (TikaException e) 
        {
            e.printStackTrace();
        }
        return null;
    }
    
}
