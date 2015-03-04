/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.lib.protocol.transfer;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.JsonNodeFactory;
import com.fasterxml.jackson.databind.node.ObjectNode;
import com.sun.media.jfxmedia.logging.Logger;
import java.io.IOException;
import java.math.BigDecimal;

/**
 *
 * @author agustin
 */
public class FileTransferInfo 
{
    private static final String ID = "id";
    private static final String INDEX = "index";
    private static final String BYTES_TO_READ = "bytesToRead";
    private static final String SEPARATOR = "&&";
    
    /**
     * @var     id
     * @brief   Es el id del archivo
     */
    private long id;
    
    /**
     * @var     index
     * @brief   Es el indice desde donde leer
     */    
    private long index;
    /**
     * @var     bytesToRead
     * @brief   Es el numero de bytes a leer
     */        
    private long bytesToRead;
    
    /**
     * @var     fileName
     * @brief   Es el path del archivo a descargar
     */
    private String fileName;

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }
    
    public FileTransferInfo()
    {
        
    }

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }
    
    /**
     * @fn  parse(String data)
     * @brief Obtengo a partir de un string la informacion solicitada
     * @param data
     * @return FileTransferInfo
     */
    public static FileTransferInfo parse(String data)
    {        
        try
        {
            //Creo un objeto
            FileTransferInfo info = new FileTransferInfo();

            String [] components = data.split(SEPARATOR);

            if(components.length <= 3 && components.length != 0)
            {
                for(String component : components)
                {
                    String key = component.substring(0, component.indexOf("="));
                    switch(key)
                    {
                        case ID:
                            info.id = Long.valueOf(component.substring(component.indexOf("=") + 1, component.length()));
                            break;

                        case INDEX:
                            info.index = Long.valueOf(component.substring(component.indexOf("=") + 1, component.length()));
                            break;

                        case BYTES_TO_READ:
                            info.bytesToRead = Long.valueOf(component.substring(component.indexOf("=") + 1, component.length()));
                            break;
                    }
                }
            }

            return info;            
        }
        catch(Exception e)
        {
            System.out.println("FileTransferInfo.parse.error: " + e.getMessage());
        }
        return null;
    }
    
    public String getJsonString()
    {
        String serialization = ID + "=" + this.id 
                               + SEPARATOR 
                               + INDEX + "=" + String.valueOf(this.index) 
                               + SEPARATOR 
                               + BYTES_TO_READ + "=" + String.valueOf(this.bytesToRead);
        return serialization;
    }

    public void setIndex(long index) {
        this.index = index;
    }

    public void setBytesToRead(long bytesToRead) {
        this.bytesToRead = bytesToRead;
    }

    public long getIndex() {
        return index;
    }

    public long getBytesToRead() {
        return bytesToRead;
    }    
}
