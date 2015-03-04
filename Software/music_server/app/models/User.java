/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package models;

import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.persistence.Entity;
import javax.persistence.Id;
import play.db.ebean.Model;


/**
 *
 * @author agustin
 */
@Entity
public class User extends Model
{
    @Id
    private Long id;
    
    private String name;
    
    private String surname;
    
    private String email;
    
    private String password;
    
    private String repassword;
    

    private static final String EMAIL_PATTERN = 
        "^[_A-Za-z0-9-\\+]+(\\.[_A-Za-z0-9-]+)*@[A-Za-z0-9-]+(\\.[A-Za-z0-9]+)*(\\.[A-Za-z]{2,})$";
    private static final Pattern pattern = Pattern.compile(EMAIL_PATTERN);
    
    private static final Finder<Long, User> finder = new Finder(Long.class, User.class);
    
    public void setId(Long id) {
        this.id = id;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setSurname(String surname) {
        this.surname = surname;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public void setRepassword(String repassword) {
        this.repassword = repassword;
    }

    public Long getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getSurname() {
        return surname;
    }

    public String getEmail() {
        return email;
    }

    public String getPassword() {
        return password;
    }

    public String getRepassword() {
        return repassword;
    }
    
    public static User isValid(String email, String password) 
    {
        return finder.where().eq("email", email).eq("password", password).findUnique();
    }
    
    public static User findById(Long userId) 
    {
        return finder.byId(userId);
    }
    
    public static void create(User user) 
    {
        user.save();
    }
    
    public static void update(User user)
    {
        user.update();
    }

    public boolean isValidUser() 
    {
        Matcher matcher = pattern.matcher(this.email);
        if(!matcher.matches())
        {
            return false;
        }
        if(this.password.length() < 5)
        {
            return false;
        }
        return true;
    }
    
    public static Object findByEmail(String email) 
    {
        return finder.where().eq("email", email);
    }
}
