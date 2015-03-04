/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package models;

import play.api.mvc.Call;

/**
 *
 * @author agustin
 */
public class RowItem 
{
    private String title;
    
    private String description;
    
    private Call link;
    
    public RowItem()
    {
    
    }
    
    public RowItem(String _title, String _description, Call _link)
    {
        title = _title;
        description = _description;
        link = _link;
    }
    

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public Call getLink() {
        return link;
    }

    public void setLink(Call link) {
        this.link = link;
    }   
    
}
