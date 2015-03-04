/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package controllers;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import models.Album;
import models.Artist;
import models.Song;
import models.User;
import play.data.DynamicForm;
import play.data.Form;
import play.mvc.Controller;
import static play.mvc.Controller.request;
import play.mvc.Result;
import static play.mvc.Results.badRequest;
import static play.mvc.Results.ok;

/**
 *
 * @author agustin
 */
public class Albums extends Controller
{
    public static Result albums()
    {
        Long userId = Long.valueOf(session().get("id"));
        
        return ok(views.html.albumsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createAlbumsList(userId)));
    }
    
    public static Result getAlbum(Long id)
    {
        Long userId = Long.valueOf(session().get("id")); 
        
        Album album = Album.get(id);
        if(album == null)
        {
            return notFound(views.html.albumsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createAlbumsList(userId)));            
        }
        List<Song> songs = Song.findListByAttributes(Arrays.asList("user.id", "album.id"), 
                                Arrays.asList((Object)userId, (Object)id));
        return ok(views.html.albumPanel.render(controllers.routes.Albums.albums(),
                                controllers.routes.Application.logout(),
                                album,
                                songs));
    }
    
    public static Result findByName()
    {
        Long userId = Long.valueOf(session().get("id")); 
        
        DynamicForm requestData = Form.form().bindFromRequest();
        String name = requestData.get("name");
        
        User user = User.findById(userId);
        
        //Filtro las canciones por lo que tengo
        List<Album> albums = Album.findBySimilarName(user, name);
        if(albums == null)
        {
            albums = new ArrayList<>();
        }
        
        return ok(views.html.albumsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            albums));        
    }        
    
    private static List<Album> createAlbumsList(Long userId)
    {
        //Obtengo las canciones por user id
        List<Album> albums = Album.findByUserId(userId);
        
        if(albums == null)
        {
            return new ArrayList<Album>();
        }
        return albums;
    }
}
