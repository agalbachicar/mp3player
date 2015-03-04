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
public class Playlist
{
    private static final String ID = "id";
    private static final String NAME = "name";
    private static final String SONGS = "songs";
    private static final String SEPARATOR = "&&";
    
    /**
     * @var     id
     * @brief   It is the id of the playlist
     */
    private int id;
    
    /**
     * @var     name
     * @brief   It is the name of the playlist
     */
    private String name;
    
    /**
     * @var     songs
     * @brief   It is the list with all the songs in it.
     */
    private List<Song> songs;
    
    /**
     * @fn      Playlist()
     * @brief   Constructor
     */
    public Playlist()
    {
    }

    /**
     * @fn      getId()
     * @brief   Getter of the id
     * @return  the id
     */
    public int getId() 
    {
        return id;
    }
    /**
     * @fn      getName()
     * @brief   Getter of the name
     * @return  the name
     */
    public String getName() 
    {
        return name;
    }
    /**
     * @fn      getSongs()
     * @brief   Getter of the songs
     * @return  List<Song>
     */
    public List<Song> getSongs() 
    {
        return songs;
    }
    /**
     * @fn      setId(int id) 
     * @brief   Setter of the id
     * @param   id 
     */
    public void setId(int id) 
    {
        this.id = id;
    }
    /**
     * @fn      setName(String name)
     * @brief   Setter of the name
     * @param   name 
     */
    public void setName(String name) 
    {
        this.name = name;
    }
    /**
     * @fn      setSongs(List<Song> songs) 
     * @brief   Setter of the songs
     * @param   songs 
     */
    public void setSongs(List<Song> songs) 
    {
        this.songs = songs;
    }
    
    public String getSerialization()
    {
        String songsStr = "";
        
        for(Song song : songs)
        {
            songsStr = songsStr + String.valueOf(song.getId()) + ",";
        }
        songsStr = songsStr.substring(0, songsStr.length() - 1);
        
        return ID + "=" + String.valueOf(this.id) +
               SEPARATOR +
               NAME + "=" + this.name +
               SEPARATOR +
               SONGS + "=[" + songsStr + "]";
    }
}
