/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package models;

import com.avaje.ebean.ExpressionList;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import javax.persistence.CascadeType;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import play.db.ebean.Model;

/**
 *
 * @author agustin
 */
@Entity
public class Album extends Model
{
    @Id
    private Long id;
    
    private String name;
    
    @ManyToOne
    private Artist artist;
    
    @ManyToOne
    private User user;
    
    private static final Finder<Long, Album> finder = new Finder(Long.class, Album.class);    

    public void setId(Long id) {
        this.id = id;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setArtist(Artist artist) {
        this.artist = artist;
    }

    public Long getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public Artist getArtist() {
        return artist;
    }
    
    public String getSerialization()
    {
        String songsStr = "";
        
        List<Song> songs = Song.findListByAttributes(Arrays.asList("user.id","album.id"), 
                    Arrays.asList((Object)user.getId(), (Object)id));
        if(songs == null)
        {
            songs = new ArrayList<>();
        }
        else if(!songs.isEmpty())
        {
            for(Song song : songs)
            {
                songsStr = songsStr + String.valueOf(song.getId()) + ",";
            }
        }

        songsStr = songsStr.substring(0, songsStr.length() - 1);
        
        return RLabels.ID + "=" + String.valueOf(this.id) +
               RLabels.SEPARATOR +
               RLabels.NAME + "=" + this.name +
               RLabels.SEPARATOR +
               RLabels.ARTIST + "=[" + artist.getSerialization() + "]" + 
               RLabels.SEPARATOR +
               RLabels.SONGS + "=[" + songsStr + "]";       
    }    
    
    public static List<Album> findByUserId(Long userId) 
    {
        return finder.where().eq("user.id", userId).findList();
    }
    
    public static List<Album> findListByAttributes(List<String> keys, List<Object> values)
    {
        ExpressionList<Album> expList = finder.where();
        
        for(int i = 0; i < keys.size(); i++)
        {
            expList.eq(keys.get(i), values.get(i));
        }
        
        return expList.findList();
    }
    
    public static Album findByAttributes(List<String> keys, List<Object> values)
    {
        ExpressionList<Album> expList = finder.where();
        
        for(int i = 0; i < keys.size(); i++)
        {
            expList.eq(keys.get(i), values.get(i));
        }
        
        return expList.findUnique();
    }     
    
    public static void create(Album album) 
    {
        album.save();
    }    

    public void setUser(User user) 
    {
        this.user = user;
    }
    
    public User getUser()
    {
        return user;
    }
    
    public static Album get(Long id) 
    {
        return finder.byId(id);
    }    
    
    public static List<Album> findBySimilarName(User user, String name)
    {
        return finder.where().eq("user.id", user.getId()).like("name", "%" + name + "%").findList();
    }    
}
