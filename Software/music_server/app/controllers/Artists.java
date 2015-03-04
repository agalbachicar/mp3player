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
import models.User;
import play.data.DynamicForm;
import play.data.Form;
import play.mvc.Controller;
import static play.mvc.Controller.request;
import play.mvc.Result;
import static play.mvc.Results.badRequest;

/**
 *
 * @author agustin
 */
public class Artists extends Controller
{
    
    public static Result artists()
    {
        Long userId = Long.valueOf(session().get("id")); 
        
        return ok(views.html.artistsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createArtistList(userId)));
    }
    
    public static Result getArtist(Long id)
    {
        Long userId = Long.valueOf(session().get("id")); 
        
        Artist artist = Artist.get(id);
        if(artist == null)
        {
            flash("error", "Artista no encontrado");
            return notFound(views.html.artistsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createArtistList(userId)));
        }
        //Obtengo los albums de los artistas
        List<Album> albums = Album.findListByAttributes(Arrays.asList("user.id", "artist.id"),
                    Arrays.asList((Object)userId,(Object)id));
        
        return ok(views.html.artistPanel.render(controllers.routes.Artists.artists(),
                                            controllers.routes.Application.logout(),
                                            artist,
                                            albums));
    }
    
    public static Result findByName()
    {
        Long userId = Long.valueOf(session().get("id")); 
        
        DynamicForm requestData = Form.form().bindFromRequest();
        String name = requestData.get("name");
        
        User user = User.findById(userId);
        
        //Filtro las canciones por lo que tengo
        List<Artist> artists = Artist.findBySimilarName(user, name);
        if(artists == null)
        {
            artists = new ArrayList<>();
        }
        
        return ok(views.html.artistsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            artists));        
    }    
    
    private static List<Artist> createArtistList(Long userId)
    {
        List<Artist> artists = Artist.findByUserId(userId);
        
        if(artists == null)
        {
            return new ArrayList<Artist>();
        }
        return artists;
    }
}
