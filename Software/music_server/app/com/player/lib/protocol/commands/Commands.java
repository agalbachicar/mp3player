/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.lib.protocol.commands;

import java.io.Serializable;

/**
 *
 * @author agustin
 */
public enum Commands implements Serializable
{   
    ACK((byte)1),
    NACK((byte)2),
    GET_NETWORK_CONFIG((byte)3),
    REGISTER((byte)4),
    DOWNLOAD_SONG((byte)5),
    GET_NEXT_SONG_DATA((byte)6),
    GET_PREVIOUS_SONG_DATA((byte)7),
    PREPAIRE_SONG((byte)8),
    DISPOSE_SONG((byte)9),
    AACK((byte)10);
        
    private final byte value;
    
    /**
     * @fn Commands(int value)
     * @param value 
     */
    private Commands(byte value)
    {
        this.value = value;
    }
    
    /**
     * @fn getValue()
     * @brief Getter
     * @return value;
     */
    public byte getValue()
    {
        return this.value;
    }    
}
