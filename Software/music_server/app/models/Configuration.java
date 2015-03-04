/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package models;

import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.OneToOne;
import play.db.ebean.Model;

/**
 *
 * @author agustin
 */
@Entity
public class Configuration extends Model
{
    @Id
    private Long id;
    
    private int udpPort;
    
    @OneToOne
    private User user;
    
    private static final Finder<Long, Configuration> finder = new Finder(Long.class, Configuration.class);
    
    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public int getUdpPort() {
        return udpPort;
    }

    public void setUdpPort(int udpPort) {
        this.udpPort = udpPort;
    }
    
    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }    
    
    public static Configuration findById(Long id)
    {
        return finder.byId(id);
    }
    
    public static void create(Configuration config)
    {
        config.save();
    }
    
    public static void delete(Configuration config)
    {
        config.delete();
    }
    
    public static void update(Configuration config)
    {
        config.update();
    }
    

    public static Configuration findByUserId(Long userId) 
    {
        return finder.where().eq("user.id", userId).findUnique();
    }    
}
