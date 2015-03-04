/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package models;

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
public class Playlist extends Model
{
    @Id
    private Long id;
    
    private String name;
    
    @ManyToOne
    private User user;
    
    private List<Song> songs; 
    
    private static final Finder<Long, Playlist> finder = new Finder(Long.class, Playlist.class);
    
    public static List<Playlist> findByUserId(Long userId) 
    {
        return finder.where().eq("user.id", userId).findList();
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public List<Song> getSongs() {
        return songs;
    }

    public void setSongs(List<Song> songs) {
        this.songs = songs;
    }    
    
    public static List<Playlist> findBySimilarName(User id, String name)
    {
        return finder.where().eq("user.id", id).like("name", "%" + name + "%").findList();
    }    
}
