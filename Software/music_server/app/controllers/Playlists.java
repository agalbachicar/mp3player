/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package controllers;

import java.util.ArrayList;
import java.util.List;
import models.Playlist;
import play.mvc.Controller;
import play.mvc.Result;

/**
 *
 * @author agustin
 */
public class Playlists extends Controller
{
    public static Result playlists()
    {
        return ok(views.html.playlistsPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createPlaylistList(1L)));
    }
    
    public static Result getPlaylist(Long id)
    {
        return TODO;
    }
    
    private static List<Playlist> createPlaylistList(Long userId)
    {
        List<Playlist> playlists = Playlist.findByUserId(userId);
        
        if(playlists == null)
        {
            return new ArrayList<Playlist>();
        }
        return playlists;
    }
}
