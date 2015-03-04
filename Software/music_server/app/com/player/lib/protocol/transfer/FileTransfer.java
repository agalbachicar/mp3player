/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.lib.protocol.transfer;

import com.player.logger.Logger;
import java.io.File;
import java.io.FileInputStream;
import java.io.RandomAccessFile;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.util.Arrays;

/**
 *
 * @author agustin
 */
public class FileTransfer 
{
    /**
     * @var     filePath
     * @brief   Path al archivo a streamear
     */
    private String filePath;

    /**
     * @var     index
     * @brief   Indice del byte desde donde hay que leer en el archivo.
     */
    private long index;


    public FileTransfer(String filePath, long index)
    {
        this.filePath = filePath;
        this.index = index;
    }

    /**
     * @return the filePath
     */
    public String getFilePath() 
    {
        return filePath;
    }

    /**
     * @param filePath the filePath to set
     */
    public void setFilePath(String filePath) 
    {
        this.filePath = filePath;
    }

    /**
     * @return the index
     */
    public long getIndex() 
    {
        return index;
    }

    /**
     * @param index the index to set
     */
    public void setIndex(long index) 
    {
        this.index = index;
    }

    /**
     * @fn      getBytes()
     * @brief   Busca el array de bytes del archivo solicitado
     * @param   buffer  es el buffer donde se dajan los datos
     * @return  La cantidad de bytes leidos
     */
    public int getBytes(byte []buffer)
    {
        try
        {
            //Creo el stream de datos
            FileInputStream fis = new FileInputStream(new File(this.filePath));
            //Me muevo la cantidad de bytes solicitados
            fis.skip(this.index);
            //Creo el array de bytes
            int bytesToRead = fis.read(buffer);
            //Cierro el stream
            fis.close();
            //Retorno la cantidad de bytes leidos
            return bytesToRead;
        }
        catch(Exception e)
        {
            Logger.log(e.getMessage());
            return -1;
        }
    }
    
    /**
     * @fn  getFileSize()
     * @brief   Devuelve la cantidad de bytes del archivo
     * @return long con la cantidad de bytes del archivo.
     * @retval  -1 en caso de error.
     */
    public long getFileSize()
    {
        try
        {
            //Creo el objeto archivo
            File file = new File(this.filePath);
            //Evaluo si el archivo existe
            if(file.exists())
            {
                return file.length();
            }
        }
        catch(Exception e)
        {
            Logger.log(e.getMessage());
        }
        return -1;
    }
    
}
