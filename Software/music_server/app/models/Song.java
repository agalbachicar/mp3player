/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package models;

import com.avaje.ebean.ExpressionList;
import com.player.lib.utils.MP3TagExtractor;
import java.io.File;
import java.io.IOException;
import static java.nio.file.StandardCopyOption.REPLACE_EXISTING;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.ManyToOne;
import play.db.ebean.Model;

/**
 *
 * @author agustin
 */
@Entity
public class Song extends Model
{    
    @Id
    private Long id;
    
    private String name;
    
    private String path;
    
    @ManyToOne
    private User user;
    
    @ManyToOne
    private Artist artist;
    
    @ManyToOne
    private Album album;
    
    private static final Finder<Long, Song> finder = new Finder(Long.class, Song.class);
    

    public void setId(Long id) {
        this.id = id;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setPath(String path) {
        this.path = path;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public void setArtist(Artist artist) {
        this.artist = artist;
    }

    public void setAlbum(Album album) {
        this.album = album;
    }

    public Long getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getPath() {
        return path;
    }

    public User getUser() {
        return user;
    }

    public Artist getArtist() {
        return artist;
    }

    public Album getAlbum() {
        return album;
    }    

    /**
     * @fn      getSerialization() 
     * @brief   It returns the song serialization
     * @return  String with song serializacion
     */
    public String getSerialization() 
    {
        return RLabels.ID + "=" + String.valueOf(this.id) + RLabels.SEPARATOR +
               RLabels.BYTES_TO_READ + "=" + new File(System.getProperty("user.dir") + this.path).length() + RLabels.SEPARATOR +
               RLabels.NAME + "=" + this.name;
    }
    
    public static List<Song> findByUserId(Long userId) 
    {
        return finder.where().eq("user.id", userId).findList();
    }
    
    public static Song get(Long id)
    {
        return finder.byId(id);
    }
    
    public static Song create(File songFile, User user) throws Exception
    {
        if(songFile == null)
        {
            throw new NullPointerException();
        }
        
        //Obtengo los tags del MP3
        Map<String, String> tags = MP3TagExtractor.getTags(songFile);
        if(tags == null)
        {
            //El archivo esta corrupto por lo que simplemente tiramos una excepcion
            throw new Exception("Archivo MP3 corrupto.");
        }
        
        Song song = new Song();
        song.setUser(user);
        song.setName(tags.get(MP3TagExtractor.TITLE));
        
        //Busco si existe un artista con el mismo nombre que pertenezca a este usuario
        Artist artist = null;
        List<Artist> artists = Artist.findListByAttributes(Arrays.asList("name", "user.id"),
                        Arrays.asList((Object)tags.get(MP3TagExtractor.ARTIST), user.getId()));
        if(artists != null && !artists.isEmpty())
        {
            artist = artists.get(0);
        }
        else
        {
            artist = new Artist();
            artist.setUser(user);
            artist.setName(tags.get(MP3TagExtractor.ARTIST));
            
            Artist.create(artist);
        }        
        song.setArtist(artist);
        
        //Busco si existe un album con el mismo nombre que pertenezca a este usuario
        Album album = null;
        List<Album> albums = Album.findListByAttributes(Arrays.asList("name", "user.id", "artist.name"),
                        Arrays.asList((Object)tags.get(MP3TagExtractor.ALBUM), (Object)user.getId(), (Object)artist.getName()));    
        if(albums != null && !albums.isEmpty())
        {
            album = albums.get(0);
        }
        else
        {
            album = new Album();
            album.setArtist(artist);
            album.setName(tags.get(MP3TagExtractor.ALBUM));
            album.setUser(user);
            
            Album.create(album);
        }           
        song.setAlbum(album);
        
        //Guardo la cancion
        song.save();
        
        //Muevo el archivo a su destino
        File destinyFile = new File(System.getProperty("user.dir") + "/fs/" + user.getId().toString() + "/" + song.getId().toString() + ".mp3");
        java.nio.file.Files.move(songFile.toPath(), destinyFile.toPath(), REPLACE_EXISTING);
        
        //Cargo el nuevo path
        song.setPath("/fs/" + user.getId().toString() + "/" + song.getId().toString() + ".mp3");
        
        //Actualizo la cancion
        song.update();
        
        return song;
    }
    
    public static List<Song> findListByAttributes(List<String> keys, List<Object> values)
    {
        ExpressionList<Song> expList = finder.where();
        
        for(int i = 0; i < keys.size(); i++)
        {
            expList.eq(keys.get(i), values.get(i));
        }
        
        return expList.findList();
    }
    
    public static Song findByAttributes(List<String> keys, List<Object> values)
    {
        ExpressionList<Song> expList = finder.where();
        
        for(int i = 0; i < keys.size(); i++)
        {
            expList.eq(keys.get(i), values.get(i));
        }
        
        return expList.findUnique();
    }    
    
    public static void delete(Song song) throws IOException 
    {
        //Elimino el archivo
        java.nio.file.Files.delete(new File((System.getProperty("user.dir") + song.getPath()).replaceAll("\\\\", "/")).toPath());
        //Elimino la entidad
        song.delete();
    }   
    
    public static List<Song> findBySimilarName(User user, String name)
    {
        return finder.where().eq("user.id", user.getId()).like("name", "%" + name + "%").findList();
    }
    
    
    public static Song nextSong(Long actualSongId)
    {
        if(actualSongId == null)
        {
            return finder.where().gt("id", 0L).orderBy("id").setMaxRows(1).findUnique();
        }
        else
        {
            return finder.where().gt("id", actualSongId).orderBy("id").setMaxRows(1).findUnique();
        }
    }
    
    public static Song previousSong(Long actualSongId)
    {
        if(actualSongId == null)
        {
            return finder.where().lt("id", 0L).orderBy("id desc").setMaxRows(1).findUnique();
        }
        else
        {
            return finder.where().lt("id", actualSongId).orderBy("id desc").setMaxRows(1).findUnique();
        }         
    }    
    
    public static Song last()
    {
        return finder.where().orderBy("id desc").setMaxRows(1).findUnique();
    }
    public static Song first()
    {
        return finder.where().orderBy("id").setMaxRows(1).findUnique();
    }
}
