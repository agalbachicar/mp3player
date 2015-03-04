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
public class SongFactory 
{
    private Song song = null;
    
    public SongFactory create()
    {
        song = new Song();
        
        return this;
    }
    
    public SongFactory setId(int id)
    {
        song.setId(id);
        
        return this;
    }
    
    public SongFactory setName(String name)
    {
        song.setName(name);
        
        return this;
    }
    
    public SongFactory setArtist(Artist artist)
    {
        song.setArtist(artist);
        
        return this;
    }
        
    public Song build()
    {
        return song;
    }
    
}
