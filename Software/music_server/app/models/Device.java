/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package models;

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
public class Device extends Model
{
    @Id
    Long id;
    
    @ManyToOne
    private User user;

    private String serial;
    
    private static final Finder<Long, Device> finder = new Finder(Long.class, Device.class);    
    

    public void setId(Long id) {
        this.id = id;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public void setSerial(String serial) {
        this.serial = serial;
    }

    public Long getId() {
        return id;
    }

    public User getUser() {
        return user;
    }

    public String getSerial() {
        return serial;
    }    
    
    public static List<Device> findByUserId(Long userId) 
    {
        return finder.where().eq("user.id", userId).findList();
    }
    
    public static Device get(Long id)
    {
        return finder.byId(id);
    }
    
    public static void delete(Device device) 
    {
        device.delete();
    }
    
    public static void create(Device device) 
    {
        device.save();
    }
    
    public static Device findBySerial(String serial) 
    {
        return finder.where().eq("serial", serial).findUnique();
    }
}
