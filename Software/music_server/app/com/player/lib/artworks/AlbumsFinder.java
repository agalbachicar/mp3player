package com.player.lib.artworks;

import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.List;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * @class AlbumsFinder
 * @author Agustin Alba Chicar
 * @brief Clase basica para buscar los albums de un artista.
 * @date 30/4/2014
 */
public class AlbumsFinder 
{
    private static final String url1 = "http://ws.audioscrobbler.com/2.0/?method=artist.gettopalbums&artist=";
    private static final String url2 = "&api_key=";
    private static final String apiKEY = "d35bed1ba2840ccd5f9e13c3b5215cec";
    
    private static final String keyALBUM = "album";
    private static final String keyNAME = "name";
    private static final String keyIMAGE = "image";
    private static final String keyGET = "GET";
    private static final String keyENCODING = "UTF-8";
    private static final String keySMALL = "small";
    private static final String keyMEDIUM = "medium";
    private static final String keyLARGE = "large";
    private static final String keyEXTRALARGE = "extralarge";
    
    
    /**
     * @fn find(String artist, String album, ImageSize size)
     * @brief   Busca todos los discos de un artista
     * @param artist    Artista a buscar.
     * @return List<String> con los discos del artista
     */    
    public static List<String> find(String artist)
    {
        //Check de las referencias iniciales
        if(artist == null || artist.isEmpty())
            return null;
        
        List<String> albums = new ArrayList<>();
        try
        {
            //Creo la URL
            String strURL = url1 + URLEncoder.encode(artist, keyENCODING) + url2 + apiKEY;
            //Creo la URL
            URL url = new URL(strURL);
            //Creo la conexion a la URL
            HttpURLConnection conn = (HttpURLConnection)url.openConnection();
            //Coloco el metodo GET
            conn.setRequestMethod(keyGET);
            //Obtengo el response code
            int responseCode = conn.getResponseCode();
            //En caso que el response code sea OK
            if(responseCode == 200)
            {
                //Bindeamos un documento XML con la informacion de la request
                DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
                DocumentBuilder builder = factory.newDocumentBuilder();
                Document doc = (Document) builder.parse(conn.getInputStream());
                //Normalizamos el docummento
                doc.getDocumentElement().normalize();
                //Nos quedamos con todos los nodos que sean del nombre albums
                NodeList nList = doc.getDocumentElement().getElementsByTagName(keyALBUM);
                //Iteramos en ellos
                for (int i = 0; i < nList.getLength(); i++) 
                {
                    //Obtengo los hijos del nodo actual e itero con ellos
                    NodeList children = nList.item(i).getChildNodes();
                    for(int j = 0; j < children.getLength(); j++)
                    {
                        //Obtengo el nodo
                        Node child = children.item(j);
                        //Evaluo el tipo del nodo
                        if(child.getNodeType() == Node.ELEMENT_NODE)
                        {
                            if(child.getNodeName().equals("name"))
                            {
                                //Agrego el nombre
                                albums.add(child.getFirstChild().getNodeValue());
                                break;
                            }
                        }
                    }
                }
            }
            else
            {   
                //Se produjo un error con el codigo de respuesta    
                return null;
            }
                    
        }
        catch(Exception e)
        {
            //En caso que se haya producido un error lo logueamos
            System.err.println(e.getMessage());
        }  
        //Si llegamos aca es porque es necesario devolver los albums encontrados
        return albums;
    }    
}
