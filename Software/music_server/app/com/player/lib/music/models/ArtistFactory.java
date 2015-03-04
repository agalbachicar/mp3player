/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.lib.music.models;

/**
 *
 * @author agustin
 */
public class ArtistFactory 
{
    private Artist artist = null;
    
    public ArtistFactory create()
    {
        artist = new Artist();
        
        return this;
    }
    
    public ArtistFactory setId(int id)
    {
        artist.setId(id);
        
        return this;
    }
    
    public ArtistFactory setName(String name)
    {
        artist.setName(name);
        
        return this;
    }
    
    public Artist build()
    {
        return artist;
    }
}
