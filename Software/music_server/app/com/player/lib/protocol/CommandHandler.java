/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.lib.protocol;

import java.net.InetAddress;

/**
 * @author agustin
 */
public interface CommandHandler 
{
    public void onCommand(byte[] data, InetAddress address, int port);
}
