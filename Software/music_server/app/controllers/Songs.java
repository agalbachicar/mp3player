/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package controllers;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import models.Song;
import models.User;
import play.data.DynamicForm;
import play.data.Form;
import play.mvc.Controller;
import play.mvc.Http;
import play.mvc.Http.MultipartFormData.FilePart;
import play.mvc.Result;

/**
 *
 * @author agustin
 */
public class Songs extends Controller
{
    public static Result songs()
    {
        Long userId = Long.valueOf(session().get("id"));
        
        return ok(views.html.songsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createSongList(userId)));
    }
    
    public static Result getSong(Long id)
    {
        Long userId = Long.valueOf(session().get("id"));
        Song song = Song.get(id);
        
        if(song == null)
        {
            flash("error", "No encontrado");
            
            return notFound(views.html.songsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createSongList(userId)));
        }
        return ok(views.html.songPanel.render(controllers.routes.Songs.songs(),
                                            controllers.routes.Application.logout(),
                                            song));        
    }
    
    
    public static Result findByName()
    {
        Long userId = Long.valueOf(session().get("id"));
        
        DynamicForm requestData = Form.form().bindFromRequest();
        String name = requestData.get("name");
        
        User user = User.findById(userId);
        
        //Filtro las canciones por lo que tengo
        List<Song> songs = Song.findBySimilarName(user, name);
        if(songs == null)
        {
            songs = new ArrayList<>();
        }
        
        return ok(views.html.songsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            songs));        
    }
    
    public static Result uploadView()
    {
        return ok(views.html.songUploadPanel.render(controllers.routes.Songs.songs(),
                                            controllers.routes.Application.logout()));        
    }
    
    public static Result uploadSong()
    {
        Long userId = Long.valueOf(session().get("id"));
        
        Http.MultipartFormData body = request().body().asMultipartFormData();
        FilePart songFilePart = body.getFile("fileToUpload");
        
        if(songFilePart == null || !songFilePart.getContentType().equals("audio/mp3"))
        {
            flash("error", "Archivo adjunto vacio.");
        }
        else
        {
            //Obtengo el archivo adjunto
            File songFile = songFilePart.getFile();
            
            User user = User.findById(userId);
            //Creo la cancion
            try
            {
                Song song = Song.create(songFile, user);
                
                if(song != null)
                {
                    return redirect(controllers.routes.Songs.getSong(song.getId()));
                }
            }
            catch(Exception e)
            {
                flash("error", e.getMessage());
            }
        }
        return ok(views.html.songsPanel.render(controllers.routes.Application.panel(),
                                    controllers.routes.Application.logout(),
                                    createSongList(userId)));
    }
    
    public static Result donwload(Long id)
    {
        Song song = Song.get(id);
        if(song == null)
        {
            return notFound();
        }
        
        return ok(new File((System.getProperty("user.dir") + song.getPath()).replaceAll("\\\\", "/")));
    }
    
    public static Result delete(Long id)
    {
        Song song = Song.get(id);
        if(song == null)
        {
            flash("error", "No encontrado");
            return notFound(views.html.songsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createSongList(1L)));
        }
        
        try
        {
            Song.delete(song);
            flash("success", "Cancion eliminada.");
        }
        catch(Exception e)
        {
            flash("error", e.getMessage());
        }
        
        return ok(views.html.songsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createSongList(1L)));        
    }
    
    private static List<Song> createSongList(Long userId)
    {
        //Obtengo las canciones por user id
        List<Song> songs = Song.findByUserId(userId);
        
        if(songs == null)
        {
            return new ArrayList<Song>();
        }
        return songs;
    }
    
    public static Result getNextSong(Long id)
    {
        Long userId = Long.valueOf(session().get("id"));
        Song song = Song.nextSong(id);
        
        if(song == null)
        {
            flash("error", "No encontrado");
            
            return notFound(views.html.songsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createSongList(userId)));
        }
        return ok(views.html.songPanel.render(controllers.routes.Songs.songs(),
                                            controllers.routes.Application.logout(),
                                            song));         
    }
    
    public static Result getPreviousSong(Long id)
    {
        Long userId = Long.valueOf(session().get("id"));
        Song song = Song.previousSong(id);
        
        if(song == null)
        {
            flash("error", "No encontrado");
            
            return notFound(views.html.songsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createSongList(userId)));
        }
        return ok(views.html.songPanel.render(controllers.routes.Songs.songs(),
                                            controllers.routes.Application.logout(),
                                            song));         
    }
}
