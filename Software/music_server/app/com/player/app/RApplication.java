/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.app;

import com.player.lib.protocol.CommandHandler;
import com.player.lib.protocol.ProtocolErrorEnum;
import com.player.lib.protocol.commands.Commands;
import com.player.lib.protocol.server.UDPProtocolServer;
import com.player.lib.protocol.transfer.FileTransferInfo;
import com.player.logger.Logger;
import java.net.InetAddress;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import models.Song;

/**
 *
 * @author agustin
 */
public class RApplication 
{
    public final static int SERVER_PORT = 15000;
    
    private static UDPProtocolServer server;
    
    //Creo el mapa de comandos con sus respectivos handlers
    private static Map<Byte, CommandHandler> commandHandlersMap = new HashMap<>();
    
    //Creo el mapa de id cancion para poder tener las canciones ya cargadas en memoria para los usuarios necesarios
    private static Map<Long, byte[]> songBufferMap = new HashMap<>();

    private static int port;
    
    private static ExecutorService executor;
    
    static
    {
        server = new UDPProtocolServer();
        //Cargo el mapa de comandos y handlers
        server.setCommandMap(commandHandlersMap);
        //Cargo el puerto de escucha
        server.setPort(SERVER_PORT);
        //Cargo el executor
        executor = Executors.newCachedThreadPool();
        server.setExecutor(executor);
        
        //Cargo el error
        server.setErrorHandler((ProtocolErrorEnum error) -> 
            {
                Logger.log("Error: " + error.name());
            });
        
        System.out.println("Arranco el server.");
        //Arranco el servidor
        server.start();    
    }

    public static int getPort() {
        return port;
    }

    public static void setPort(int port) {
        RApplication.port = port;
    }

    public static void log(String str)
    {
        Logger.log("SERVER: " + str);
    }
    
    public static void populateCommandsHandlers()
    {
        //Para cada comando creo un handler
        commandHandlersMap.put(Commands.ACK.getValue(),
                (CommandHandler) (byte[] data, InetAddress address, int port) -> 
                {
                    Logger.log("SERVER: ACK");
                    //Respondo con ACK
                    server.send(null, Commands.ACK.getValue(), address, port);                    
                });

        commandHandlersMap.put(Commands.AACK.getValue(),
                (CommandHandler) (byte[] data, InetAddress address, int port) -> 
                {
                    Logger.log("SERVER: AACK");
                    //Respondo con ACK
                    server.send(null, Commands.ACK.getValue(), address, port);                    
                });        
        commandHandlersMap.put(Commands.PREPAIRE_SONG.getValue(), 
            (CommandHandler) (byte[] data, InetAddress address, int port) -> 
            {
                Logger.log("SERVER: PREPAIRE_SONG");
                //Levanto la cancion a RAM
                try
                {
                    FileTransferInfo info = FileTransferInfo.parse(new String(data));
                    if(info == null)
                    {
                        server.send(null, Commands.NACK.getValue(), address, port);
                        return;
                    }                    
                    Long songId = info.getId();
                    //Obtengo la cancion
                    Song song = Song.get(songId);
                    if(song == null)
                    {
                        server.send(null, Commands.NACK.getValue(), address, port);
                        return;
                    }
                    else if(songBufferMap.containsKey(songId))
                    {
                        //Respondo con ACK
                        server.send(null, Commands.ACK.getValue(), address, port);
                        return;
                    }
                    //Paso el archivo a bytes
                    Path path = Paths.get((System.getProperty("user.dir") + song.getPath()).replaceAll("\\\\", "/"));
                    //Obtengo los bytes
                    byte[] byteBuffer = Files.readAllBytes(path);
                    //Cargo la cancion al mapa
                    if(!songBufferMap.containsKey(songId))
                    {
                        songBufferMap.put(songId, byteBuffer);
                    }
                    
                    //Respondo con ACK
                    server.send(null, Commands.ACK.getValue(), address, port);
                }
                catch(Exception e)
                {
                    Logger.log("SERVER: " + e.getMessage());
                    server.send(null, Commands.NACK.getValue(), address, port);
                }
            });
        commandHandlersMap.put(Commands.DISPOSE_SONG.getValue(),
            (CommandHandler) (byte[] data, InetAddress address, int port) -> 
            {
                Logger.log("SERVER: DISPOSE_SONG");     
                //Bajo la cancion de RAM
                try
                {
                    FileTransferInfo info = FileTransferInfo.parse(new String(data));
                    if(info == null)
                    {
                        server.send(null, Commands.NACK.getValue(), address, port);
                        return;
                    }                    
                    Long songId = info.getId();
                    //Cargo la cancion al mapa
                    if(!songBufferMap.containsKey(songId))
                    {
                        songBufferMap.remove(songId);
                    }
                    //Respondo con ACK
                    server.send(null, Commands.ACK.getValue(), address, port);
                }
                catch(Exception e)
                {
                    Logger.log("SERVER: " + e.getMessage());
                    server.send(null, Commands.NACK.getValue(), address, port);
                }                    
            });  
                
        commandHandlersMap.put(Commands.DOWNLOAD_SONG.getValue(),
            (CommandHandler) (byte[] data, InetAddress address, int port) -> 
            {   
                //Logger.log("SERVER: DOWNLOAD_SONG");
                //Convierto a un string lo que me envian y de allí obtengo
                //el nombre de cancion y el indice desde donde leer, ademas 
                //de la cantidad de bytes solicitados
                //Creo un objeto con los datos deserializados
                try
                {
                    FileTransferInfo info = FileTransferInfo.parse(new String(data));
                    if(info == null)
                    {
                        server.send(null, Commands.NACK.getValue(), address, port);
                        return;
                    }
                    //Busco la cancion y si esta cacheado
                    if(!songBufferMap.containsKey(info.getId()))
                    {
                        server.send(null, Commands.NACK.getValue(), address, port);
                        return;                    
                    }

                    //Obtengo el array solicitado
                    byte [] songBuffer = songBufferMap.get(info.getId());                
                    byte [] responseBuffer;
                    //Copio la parte del array que me piden o que puedo
                    if(info.getBytesToRead() <= (songBuffer.length - info.getIndex()))
                    {
                        responseBuffer = Arrays.copyOfRange(songBuffer,
                                          (int)info.getIndex(),
                                          (int)(info.getIndex() + info.getBytesToRead()));
                    }
                    else
                    {
                        responseBuffer = Arrays.copyOfRange(songBuffer,
                                          (int)info.getIndex(),
                                          songBuffer.length);                    
                    }
                    server.send(responseBuffer ,Commands.ACK.getValue(), address, port);                               
                }
                catch(Exception e)
                {
                    server.send(null, Commands.NACK.getValue(), address, port);                               
                }
            });
                
        commandHandlersMap.put(Commands.GET_NEXT_SONG_DATA.getValue(),
                (CommandHandler) (byte[] data, InetAddress address, int port) -> 
                {
                    try
                    {
                        Song song = null;
                        FileTransferInfo info = FileTransferInfo.parse(new String(data));
                        if(info == null)
                        {
                            song = Song.first();
                        }                        
                        else
                        {
                            song = Song.nextSong(info.getId());
                        }
                        //Evaluo el resultado
                        if(song == null)
                        {
                            //Puede que hoy haya mas, evaluo entonces la primera cancion
                            //para saber si hay al menos una
                            song = Song.first();
                            if(song == null)
                            {
                                //Respondo con ACK
                                server.send(null, Commands.ACK.getValue(), address, port);
                            }
                            else
                            {
                                //Respondo con ACK
                                server.send(song.getSerialization().getBytes(), Commands.ACK.getValue(), address, port);                                
                            }
                        }
                        else
                        {
                            Logger.log("SERVER: GET_NEXT_SONG_DATA. Cancion: " + song.getName());   
                            //Respondo con ACK
                            server.send(song.getSerialization().getBytes(), Commands.ACK.getValue(), address, port);
                        }
                    }
                    catch(Exception e)
                    {
                        Logger.log("SERVER: GET_NEXT_SONG_DATA. Error");   
                        server.send(null, Commands.NACK.getValue(), address, port);
                    }
                });
        
        commandHandlersMap.put(Commands.GET_PREVIOUS_SONG_DATA.getValue(),
                (CommandHandler) (byte[] data, InetAddress address, int port) -> 
                {
                    Logger.log("SERVER: GET_PREVIOUS_SONG_DATA");
                    
                    try
                    {
                        Song song = null;
                        FileTransferInfo info = FileTransferInfo.parse(new String(data));
                        if(info == null)
                        {
                            song = Song.last();
                        }
                        else
                        {
                            song = Song.previousSong(Long.valueOf(info.getId()));
                        }
                        //Evaluo el resultado
                        if(song == null)
                        {
                            //Respondo con ACK
                            server.send(null, Commands.ACK.getValue(), address, port);                                
                        }
                        else
                        {
                            //Respondo con ACK
                            server.send(song.getSerialization().getBytes(), Commands.ACK.getValue(), address, port);
                        }
                    }
                    catch(Exception e)
                    {
                        server.send(null, Commands.NACK.getValue(), address, port);
                    }                    
                });           
        
        commandHandlersMap.put(Commands.GET_NETWORK_CONFIG.getValue(),
                (CommandHandler) (byte[] data, InetAddress address, int port) -> 
                {
                    Logger.log("SERVER: GET_NETWORK_CONFIG");
                });                                        
        
        commandHandlersMap.put(Commands.NACK.getValue(),
                (CommandHandler) (byte[] data, InetAddress address, int port) -> 
                {
                    Logger.log("SERVER: NACK");
                });        
        
        commandHandlersMap.put(Commands.REGISTER.getValue(),
                (CommandHandler) (byte[] data, InetAddress address, int port) -> 
                {
                    Logger.log("SERVER: REGISTER");
                });        
        
        server.setCommandMap(commandHandlersMap);
    }
    
    public static void createExcecutor()
    {
        executor = Executors.newCachedThreadPool();
        server.setExecutor(executor);
    }
    
    public static void startServer()
    {
        log("PORT: " + port);
        log("START");
        //Arranco el servidor
        server.start();    
    }
    
    public static void stopServer()
    {
        //Apago los threads
        executor.shutdown();
        while(!executor.isShutdown())
        {
        }
    }
}
