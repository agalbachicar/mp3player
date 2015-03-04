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
public class Song 
{  
    private static String ID = "id";
    private static String NAME = "name";
    private static String ARTIST = "artist";
    private static String SEPARATOR = "&&";
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
     * @fn      Song()
     * @brief   Constructor
     */
    public Song()
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
     * @fn      setArtist(Artist artist)
     * @brief   Setter for the artist
     * @param   artist 
     */
    public void setArtist(Artist artist) 
    {
        this.artist = artist;
    } 
 

    /**
     * @fn      getSerialization() 
     * @brief   It returns the song serialization
     * @return  String with song serializacion
     */
    public String getSerialization() 
    {
//        return ID + "=" + String.valueOf(this.id) +
//               NAME + "=" + this.name +
//               ARTIST + "=[" + this.artist.getSerialization() + "]";
        return "";
    }
    
}
