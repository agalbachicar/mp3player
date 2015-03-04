/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package models;

import com.avaje.ebean.ExpressionList;
import java.util.List;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.ManyToOne;
import play.db.ebean.Model;

/**
 *
 * @author agustin
 */
@Entity
public class Artist extends Model
{
    @Id
    private Long id;
    
    private String name;
    
    @ManyToOne
    private User user;
    
    private static final Finder<Long, Artist> finder = new Finder(Long.class, Artist.class);    
    

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
    
    /**
     * @fn      getSerialization()
     * @brief   It returns the String representation of the artist
     * @return  String representation of the artist
     */
    public String getSerialization() 
    {
        return RLabels.ID + "=" + String.valueOf(this.id) +
               RLabels.SEPARATOR +
               RLabels.NAME + "=" + String.valueOf(this.name);
    }
    
    public static List<Artist> findByUserId(Long userId)
    {
        return finder.where().eq("user.id", userId).findList();
    }
    
    public static List<Artist> findListByAttributes(List<String> keys, List<Object> values)
    {
        ExpressionList<Artist> expList = finder.where();
        
        for(int i = 0; i < keys.size(); i++)
        {
            expList.eq(keys.get(i), values.get(i));
        }
        
        return expList.findList();
    }
    
    public static Artist findByAttributes(List<String> keys, List<Object> values)
    {
        ExpressionList<Artist> expList = finder.where();
        
        for(int i = 0; i < keys.size(); i++)
        {
            expList.eq(keys.get(i), values.get(i));
        }
        
        return expList.findUnique();
    }    
    
    public static void create(Artist artist)
    {
        artist.save();
    }
    
    public static Artist get(Long id) 
    {
        return finder.byId(id);
    }    
    
    public static List<Artist> findBySimilarName(User user, String name)
    {
        return finder.where().eq("user.id", user.getId()).like("name", "%" + name + "%").findList();
    }    
}
