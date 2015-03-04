/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.lib.protocol.server;

import com.player.lib.protocol.CommandHandler;
import com.player.lib.protocol.ProtocolErrorEnum;
import com.player.lib.protocol.ProtocolErrorHandler;
import java.net.InetAddress;
import java.util.Map;
import java.util.concurrent.Executor;

/**
 *
 * @author agustin
 */
public abstract class ProtocolServer 
{
    //Header del protocolo
    protected static final byte HEADER = (byte) 0xAA;
    //Footer del protocolo
    protected static final byte FOOTER = (byte) 0xEE;
    //Mapa de comandos y handler de los  comandos
    protected Map<Byte, CommandHandler> commandMap;
    //Callback de error del protocolo
    protected ProtocolErrorHandler errorHandler; 
    //Executor para la emision de eventos
    protected Executor executor;    
    
    
    //Metodos abstractos a implementar
    public abstract void send(byte[] buffer, byte command, InetAddress address, int port);
    public abstract void start();
    
    
    /**
     * @fn setErrorHandler(ProtocolErrorHandler errorHandler)
     * @brief Setter del handler de error
     * @param errorHandler 
     */
    
    public void setErrorHandler(ProtocolErrorHandler errorHandler) 
    {
        this.errorHandler = errorHandler;
    }
    
    
    /**
     * @fn checkChecksum(byte[] data, byte checksumSent)
     * @brief Evaluo el checksum enviado
     * @param data
     * @param checksumSent
     * @return boolean
     */
    protected boolean checkChecksum(byte[] data, byte checksumSent)
    {
        //Comparo el computado contre el enviado
        return (computeChecksum(data) == checksumSent);
    }
    
    /**
     * @fn computeChecksum(byte[] data)
     * @brief Computo el checksum
     * @param data
     * @return byte
     */
    protected byte computeChecksum(byte[] data)
    {
        //Check de los datos
        if(data == null || data.length == 0)
            return 0x00;
        
        //Calculo el checksum de los datos a enviar
        byte computedChecksum = 0x00;
        for(byte dataByte : data)
            computedChecksum ^= dataByte;
        
        //Retorno el checksum computado
        return computedChecksum;
    }
    
    /**
     * @fn setCommandMap(Map<Byte, CommandHandler> commandMap)
     * @brief Setter del command map
     * @param commandMap 
     */
    public void setCommandMap(Map<Byte, CommandHandler> commandMap)
    {
       this.commandMap = commandMap; 
    }
    
    /**
     * @fn packageConstructor(byte[] data, byte command)
     * @param data
     * @param command
     * @return byte[]
     */
    protected byte[] packageConstructor(byte[] data, byte command)
    {
        try
        {
            if(data == null)
            {
                data = new byte[0];
            }
            //Computo el checksum
            byte checksum = computeChecksum(data);
            //Genero el byte de la salida
            //                                H + CMD + SIZE +     DATA    + CHKSUM + F
            byte[] packageInBytes = new byte[ 1 +  1  +  2   + data.length + 1      + 1];
            
            //Comienzo la copia
            packageInBytes[0] = HEADER;
            packageInBytes[1] = command;
            //Error!!!!!
            //Array.setInt(data, 2, data.length);
            packageInBytes[2] = (byte) (data.length % 256);
            packageInBytes[3] = (byte) (data.length / 256);
            System.arraycopy(data, 0, packageInBytes, 4, data.length);
            packageInBytes[packageInBytes.length - 2] = checksum;
            packageInBytes[packageInBytes.length - 1] = FOOTER;
            
            return packageInBytes;
        }
        catch(Exception e)
        {
            return null;
        }
    }
    
    /**
     * @fn setExecutor(Executor executor)
     * @brief Setter del executor
     * @param executor 
     */
    public void setExecutor(Executor executor)
    {
        this.executor = executor;
    }    
    
    /**
     * @fn runErrorCallback(ProtocolErrorEnum errorType)
     * @brief Check del handler de error por si es null y lo ejecuta en un 
     *         en un thread a parte si se puede
     * @param errorType 
     */
    protected void runErrorCallback(ProtocolErrorEnum errorType)
    {
        if(errorHandler != null)
        {
            if(executor != null)
            {
                executor.execute(new Runnable() 
                {
                    @Override
                    public void run() 
                    {
                        errorHandler.onError(errorType);
                    }
                });
            }
            else
            {
                errorHandler.onError(errorType);
            }
        }        
    }
}
