/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.lib.protocol;

/**
 *
 * @author agustin
 */
public enum ProtocolErrorEnum 
{
    BAD_HEADER_FOOTER,
    UNKNOWN_CMD,
    BAD_PACKAGE_SIZE,
    BAD_CHECKSUM,
    NETWORK, 
    UNKNOWN,
}
