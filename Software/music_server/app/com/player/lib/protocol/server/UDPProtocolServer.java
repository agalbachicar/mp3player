/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.lib.protocol.server;

import com.player.lib.protocol.CommandHandler;
import com.player.lib.protocol.ProtocolErrorEnum;
import com.player.lib.udp.sync.UDPErrorCallback;
import com.player.lib.udp.sync.UDPReceptionCallback;
import com.player.lib.udp.sync.UDPServer;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.util.Arrays;
import java.util.concurrent.Executor;

/**
 *
 * @author agustin
 */
public class UDPProtocolServer extends ProtocolServer
{
    //Servidor UDP
    private final UDPServer server;
    //Handler de error del servidor UDP
    private final UDPErrorCallback lowLevelErrorHandler;
    //Handler de recepcion del servidor UDP
    private final UDPReceptionCallback lowLevelReceptionHandler;
    
    /**
     * @fn UDPProtocolServer()
     * @brief Constructor
     */
    public UDPProtocolServer()
    {
        //Creo el servidor
        server = new UDPServer();
        //Handler de error del nivel bajo
        lowLevelErrorHandler = new UDPErrorCallback() 
        {
            @Override
            public void onError(Exception e) 
            {
                System.out.println("onError: " + e.getMessage());
                runErrorCallback(ProtocolErrorEnum.NETWORK);
            }
        };
        //Handler de recepcion del nivel bajo
        lowLevelReceptionHandler = new UDPReceptionCallback() 
        {
            @Override
            public void onReceive(DatagramPacket receivePacket) 
            {
                //Obtengo los datos del paquete y los proceso
                processPackage(receivePacket.getData(), receivePacket.getAddress(), receivePacket.getPort());
            }
        };
        
        server.setErrorCallback(lowLevelErrorHandler);
        server.setReceptionCallback(lowLevelReceptionHandler);
        server.setReceiveBufferSize(1024);
    }
    
    /**
     * @fn setPort(int port)
     * @brief Setter del port
     * @param port 
     */
    public void setPort(int port)
    {
        this.server.setPort(port);
    }
    
    /**
     * @fn setExecutor(Executor executor)
     * @brief Setter del executor
     * @param executor 
     */
    @Override
    public void setExecutor(Executor executor)
    {
        //Cargo el executor en el servidor UDP
        this.server.setExecutor(executor);
        //Lo envio al propio objeto padre
        super.setExecutor(executor);
    }    

    
    /**
     * @fn send(byte[] buffer, byte command, InetAddress address, int port) 
     * @param data
     * @param command
     * @param address
     * @param port 
     */
    @Override
    public void send(byte[] data, byte command, InetAddress address, int port) 
    {
        //Evaluo que la direccion sea valida asi como el puerto
        if(address == null || port < 1)
            return;
        //Construyo el paquete
        byte[] packageConstructor = this.packageConstructor(data, command);
        //Envio el paquete
        this.server.async_write(packageConstructor, address, port);
    }

    /**
     * @fn start() 
     * @brief It starts to listen packets
     */
    @Override
    public void start() 
    {
        this.server.listen();
    }
    
    /**
     * @fn processPackage(byte[] data, InetAddress senderAddress)
     * @brief Se procesan los datos recibidos.
     * @param data recibida
     * @param senderAddress es la address del origen de los datos 
     */
    private void processPackage(byte[] data, InetAddress senderAddress, int port)
    {
        try
        {
            if(data == null || data.length < 6)
            {
                //Se produjo un error con este paquete ya que debiera tener datos
                runErrorCallback(ProtocolErrorEnum.BAD_PACKAGE_SIZE);
                return;
            }

            // 1.- Obtencion y verificacion del header y footer
            if(data[0] != HEADER)
            {
                //Error de header
                runErrorCallback(ProtocolErrorEnum.BAD_HEADER_FOOTER);
                return;
            }
            
            // 2.- Verifico que se contenga el comando recibido en el mapa
            if(!this.commandMap.containsKey(data[1]))
            {
                //No se reconocio el comando
                runErrorCallback(ProtocolErrorEnum.UNKNOWN_CMD);
                return;
            }

            //3.- Obtengo el numero de bytes
            byte[] numberOfBytes = new byte[2];
            numberOfBytes[0] = data[2];
            numberOfBytes[1] = data[3];
            int dataLength = ((numberOfBytes[1] << 8) & 0x0000FF00) | (numberOfBytes[0] & 0x000000FF);

            //Genero el checksum
            // 4.- Ojo si recibi 0 de dataLength
            byte[] packageData = Arrays.copyOfRange(data, 4, 4 + dataLength);
            if(!checkChecksum(packageData, data[4 + dataLength]))
            {
                //No coincidio el checksum!
                runErrorCallback(ProtocolErrorEnum.BAD_CHECKSUM);
                return;
            }

            // 5.- Emito el evento de fin de recepcion
            final CommandHandler cmdHandler = (CommandHandler)commandMap.get(data[1]);
            
            if(cmdHandler != null)
            {
                if(executor != null)
                {
                    executor.execute(new Runnable() 
                    {
                        @Override
                        public void run() 
                        {
                            cmdHandler.onCommand(packageData, senderAddress, port);                
                        }
                    });
                }
                else
                {
                    cmdHandler.onCommand(packageData, senderAddress, port);                
                }            
            }
        }
        catch(Exception e)
        {
            System.out.println("UDPProtocolServer.processPackage: " + e.getMessage());
            e.printStackTrace();
            runErrorCallback(ProtocolErrorEnum.UNKNOWN);
        }
    }
}

