/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.lib.udp.async;


import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.concurrent.Executor;

/**
 *
 * @author agustin
 */
public class UDPServer 
{
    private int port;
    private int receiveBufferSize;
    
    private Executor exec;
    
    private UDPReceptionCallback receptionCallback;
    private UDPErrorCallback errorCallback;
    
    private static final int defaultPort = 10000;
    private static final int defaultReceiveBufferSize = 1024;
    
    private DatagramSocket socketServer;
    
    
    /**
     * @fn UDPServer ()
     * @brief Constructor
     */
    public UDPServer ()
    {
        //Inicializo las variables con los valores por default
        port = defaultPort;
        receiveBufferSize = defaultReceiveBufferSize;
        
        //Cargo el executor a null
        exec = null;
        
        //Coloco a null los callbacks
        receptionCallback = null;
        errorCallback = null;
    }
    
    /**
     * @fn setPort(int port)
     * @brief Setter del port a escuchar
     * @param port Es el puerto a escuchar
     */
    public void setPort(int port)
    {
        this.port = port;
    }
    
    /**
     * @fn setReceiveBufferSize(int receiveBufferSize)
     * @brief   Setter del buffer size de recepcion
     * @param receiveBufferSize Es el tamaño de bytes a recibir.
     */
    public void setReceiveBufferSize(int receiveBufferSize)
    {
        this.receiveBufferSize = receiveBufferSize;
    }
    
    /**
     * @fn setExecutor(Executor exec)
     * @brief Cargar un pool de threads para poder threadear la escucha.
     * @param exec Executor para threadear la escucha
     */
    public void setExecutor(Executor exec)
    {
        this.exec = exec;
    }
    
    /**
     * @fn setReceptionCallback(UDPReceptionCallback receptionCallback)
     * @brief Setter del callback de recepcion
     * @param receptionCallback 
     */
    public void setReceptionCallback(UDPReceptionCallback receptionCallback)
    {
        this.receptionCallback = receptionCallback;
    }
    
    /**
     * @fn setErrorCallback(UDPErrorCallback errorCallback)
     * @brief Setter del callback de error
     * @param errorCallback 
     */
    public void setErrorCallback(UDPErrorCallback errorCallback)
    {
        this.errorCallback = errorCallback;
    }
    
    /**
     * @fn listen()
     * @brief Comienza a escuchar el server 
     */
    public void listen()
    {
        try
        {
            //Abro un thread y ejecuto el server
            exec.execute(new Runnable()
            {
                @Override
                public void run() 
                {
                    //Evaluo si el socket esta abierto y lo cierro
                    if(socketServer != null)
                    {   
                        socketServer.close();
                    }
                    
                    try
                    {
                        //Creo un socket datagrama en el puerto indicado
                        socketServer = new DatagramSocket(port);         
                        
                        //Repito indefinidamente la accion de escuchar datos y emitir acciones
                        while(true)
                        {
                            //Creo un buffer del tamaño indicado
                            byte[] receiveBuffer = new byte[receiveBufferSize];
                            //Creo el paquete datagrama
                            DatagramPacket receivePacket = new DatagramPacket(receiveBuffer, receiveBufferSize);
                            
                            //Escucho por el server
                            socketServer.receive(receivePacket);
                            //Emito el evento de paquete recibido
                            if(receptionCallback != null)
                            {
                                exec.execute(new Runnable()
                                {
                                    @Override
                                    public void run() 
                                    {
                                        try
                                        {
                                            receptionCallback.onReceive(receivePacket);
                                        }
                                        catch(Exception e)
                                        {
                                        
                                        }
                                    }
                                });
                            }
                        }
                    }
                    catch(Exception e)
                    {
                        //Emito el evento de error
                        if(errorCallback != null)
                        {
                            errorCallback.onError(e);
                        }
                    }
                }
            });
        }
        catch(Exception e)
        {
            //Emito el evento de error
           errorCallback.onError(e);
        }
    }
    
    /**
     * @fn write(byte[] sendBuffer, InetAddress clientAddress, int port)
     * @param sendBuffer es el buffer a enviar
     * @param clientAddress es la direccion a la que se desea enviar
     * @param port al que se desea enviar
     * @return boolean con el estado de la operacion
     */
    public boolean write(byte[] sendBuffer, InetAddress clientAddress, int port)
    {
        //Evaluo el buffer a enviar
        if(sendBuffer == null)
            return false;
        
        //Check si el scoketServer esta siendo utilizado
        if(socketServer == null)
        {
            //Creo el socket
            try
            {
                socketServer = new DatagramSocket(port);
            }
            catch(Exception e)
            {
                return false;
            }
        }
        
        //Creo un paquete datagrama para enviar
        DatagramPacket sendPacket = new DatagramPacket(sendBuffer,
                                   sendBuffer.length,
                                   clientAddress,
                                   port);
        //Envio el paquete
        try
        {
            socketServer.send(sendPacket);
        }
        catch(Exception e)
        {
            return false;
        }
        
        return true;
    }
    
    
    /**
     * @fn write(String sendString, InetAddress clientAddress, int port)
     * @param sendString es la string a enviar
     * @param clientAddress es la address de destino
     * @param port al que uno se desea comunicar
     * @return boolean con el estado de la operacion
     */
    public boolean write(String sendString, InetAddress clientAddress, int port)
    {
        if(sendString == null || sendString.isEmpty())
            return false;
        
        return write(sendString.getBytes(), clientAddress, port);
    }
    
    /**
     * @fn async_write(final byte[] sendBuffer, final InetAddress clientAddress, final int port)
     * @brief Envia un buffer de forma asincronica por el servidor.
     * @param sendBuffer buffer a enviar datos
     * @param clientAddress es la direccion de envio
     * @param port al que se desea enviar
     */
    public void async_write(final byte[] sendBuffer, final InetAddress clientAddress, final int port)
    {
        try
        {
            exec.execute(new Runnable() 
            {
                @Override
                public void run() 
                {
                    write(sendBuffer, clientAddress, port);
                }
            });
        }
        catch(Exception e)
        {
        
        }
    }
    
    /**
     * @fn async_write(final String sendBsendStringuffer, final InetAddress clientAddress, final int port)
     * @brief Envia un buffer de forma asincronica por el servidor.
     * @param sendString String a enviar datos
     * @param clientAddress es la direccion de envio
     * @param port al que se desea enviar
     */    
    public void async_write(final String sendString, final InetAddress clientAddress, final int port)
    {
        try
        {
            exec.execute(new Runnable() 
            {
                @Override
                public void run() 
                {
                    write(sendString.getBytes(), clientAddress, port);
                }
            });
        }
        catch(Exception e)
        {
        
        }
    }    

    
}
