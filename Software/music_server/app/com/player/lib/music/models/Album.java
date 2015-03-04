/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package com.player.lib.music.models;

import java.util.List;

/**
 * @author agustin
 */
public class Album
{
    private static final String ID = "id";
    private static final String NAME = "name";
    private static final String ARTIST = "artist";
    private static final String SONGS = "songs";
    private static final String SEPARATOR = "&&";
    
    /**
     * @var     id
     * @brief   It is the id
     */
    private int id;
    /**
     * @var     name
     * @brief   It is the name of the album
     */
    private String name;
    /**
     * @var     artist
     * @brief   The artist to which it belongs to
     */ 
    private Artist artist;
    /**
     * @var     songs
     * @brief   The list of songs in this album
     */
    private List<Song> songs;
    
    /**
     * @fn      Album()
     * @brief   Constructor
     */
    public Album()
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
     * @fn      getArtist() 
     * @brief   Getter of the artist
     * @return  the artist
     */
    public Artist getArtist() 
    {
        return artist;
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
    /**
     * @fn      setArtist(Artist artist)
     * @brief   Setter for the artist
     * @param   artist 
     */
    public void setArtist(Artist artist) 
    {
        this.artist = artist;
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
               ARTIST + "=[" + artist.getSerialization() + "]" + 
               SEPARATOR +
               SONGS + "=[" + songsStr + "]";       
    }
}
