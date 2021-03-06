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
public class Artist 
{
    
    private static final String ID = "id";
    private static final String NAME = "name";
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
     * @fn      Artist()
     * @brief   Constructor
     */
    public Artist()
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
     * @fn      getSerialization()
     * @brief   It returns the String representation of the artist
     * @return  String representation of the artist
     */
    public String getSerialization() 
    {
        return ID + "=" + String.valueOf(this.id) +
               SEPARATOR +
               NAME + "=" + String.valueOf(this.name);
    }
}
