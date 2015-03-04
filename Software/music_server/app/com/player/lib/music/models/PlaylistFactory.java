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
public class PlaylistFactory 
{
    private Playlist playlist = null;
    
    
    public PlaylistFactory create()
    {
        playlist = new Playlist();
        
        return this;
    }
    
    public PlaylistFactory setId(int id)
    {
        playlist.setId(id);
        
        return this;
    }
    public PlaylistFactory setName(String name)
    {
        playlist.setName(name);
        
        return this;
    }    
    public PlaylistFactory setSongs(List<Song> songs)
    {
        playlist.setSongs(songs);
        
        return this;
    }
    public Playlist build()
    {
        return playlist;
    }
    
}
