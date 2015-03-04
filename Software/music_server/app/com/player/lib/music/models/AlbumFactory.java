/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.lib.music.models;

import java.util.List;

/**
 *
 * @author agustin
 */
public class AlbumFactory 
{
    private Album album = null;
    
    public AlbumFactory create()
    {
        album = new Album();
        
        return this;
    }
    
    public AlbumFactory setId(int i)
    {
        album.setId(i);
        
        return this;
    }
    
    public AlbumFactory setName(String name)
    {
        album.setName(name);
        
        return this;
    }
    
    public AlbumFactory setArtist(Artist artist)
    {
        album.setArtist(artist);
        
        return this;
    }
    
    public AlbumFactory setSongs(List<Song> songs)
    {
        album.setSongs(songs);
        
        return this;
    }
    
    public Album build()
    {
        return album;
    }
}


