/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.lib.udp.sync;


import java.net.DatagramPacket;

/**
 *
 * @author agustin
 */
public interface UDPReceptionCallback 
{
    public void onReceive(DatagramPacket receivePacket);
}
