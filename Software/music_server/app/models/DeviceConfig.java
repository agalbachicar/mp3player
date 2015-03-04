/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package models;

import com.fasterxml.jackson.annotation.JsonIgnore;
import java.net.InetAddress;

/**
 *
 * @author agustin
 */
public class DeviceConfig 
{
    @JsonIgnore
    private Long id;

    private String ip;
    private String netmask;
    private String gateway;
    private String serverIP;
    private int port;
    
    private static byte[] mac;
    
    static
    {
        mac = new byte[6];
        mac[0] = (byte)0x02;
        mac[1] = (byte)0x01;
        mac[2] = (byte)0x01;
        mac[3] = (byte)0x00;
        mac[4] = (byte)0x00;
        mac[5] = (byte)0x00;
    }
    
    public DeviceConfig()
    {
        mac[5]++;
    }

    
    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getIp() {
        return ip;
    }

    public void setIp(String ip) {
        this.ip = ip;
    }

    public String getNetmask() {
        return netmask;
    }

    public void setNetmask(String netmask) {
        this.netmask = netmask;
    }

    public String getGateway() {
        return gateway;
    }

    public void setGateway(String gateway) {
        this.gateway = gateway;
    }

    public String getServerIP() {
        return serverIP;
    }

    public void setServerIP(String serverIP) {
        this.serverIP = serverIP;
    }

    public int getPort() {
        return port;
    }

    public void setPort(int port) {
        this.port = port;
    }  

    public byte[] toBytes() 
    {
        try
        {
            //Creamos el buffer para serializar
                                          /*ip + netmask + gateway + mac + port + id + serverIP*/
            byte []reponseBuffer = new byte[4  + 4       + 4       + 6   + 2    /*+ 4*/  + 4];

            byte [] address;
            //Copio la IP
            address = InetAddress.getByName(this.ip).getAddress();
            reponseBuffer[0] = address[0];
            reponseBuffer[1] = address[1];
            reponseBuffer[2] = address[2];
            reponseBuffer[3] = address[3];

            //Copio la netmask
            address = InetAddress.getByName(this.netmask).getAddress();
            reponseBuffer[4] = address[0];
            reponseBuffer[5] = address[1];
            reponseBuffer[6] = address[2];
            reponseBuffer[7] = address[3];
            
            //Copio el gateway
            address = InetAddress.getByName(this.gateway).getAddress();
            reponseBuffer[8] = address[0];
            reponseBuffer[9] = address[1];
            reponseBuffer[10] = address[2];
            reponseBuffer[11] = address[3];
            
            //Copio la mac
            reponseBuffer[12] = mac[0];
            reponseBuffer[13] = mac[1];
            reponseBuffer[14] = mac[2];
            reponseBuffer[15] = mac[3];
            reponseBuffer[16] = mac[4];
            reponseBuffer[17] = mac[5];            

            //Copio el puerto
            reponseBuffer[18] = (byte)(this.port         & 0xFF);
            reponseBuffer[19] = (byte)((this.port >> 8)  & 0xFF);

            //Copio el serverIP
            address = InetAddress.getByName(this.serverIP).getAddress();
            reponseBuffer[20] = address[0];
            reponseBuffer[21] = address[1];
            reponseBuffer[22] = address[2];
            reponseBuffer[23] = address[3];
            
            return reponseBuffer;        
        }
        catch(Exception e)
        {
            return null;
        }
    }
}
