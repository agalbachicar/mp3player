package com.player.lib.artworks;


import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 * @class ArtboardFinder
 * @author Agustin Alba Chicar
 * @brief Clase basica para buscar las URL de los artboards de un album.
 * @date 30/4/2014
 */
public class ArtboardFinder 
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
     * @enum ImageSize
     * @brief Diferentes tamaños de imagenes que se pueden conseguir
     */
    public enum ImageSize
    {
        SMALL,
        MEDIUM,
        LARGE,
        EXTRALARGE
    }
    
    /**
     * @fn find(String artist, String album, ImageSize size)
     * @brief   Busca la URL de una imagen de un disco.
     * @param artist    Artista a buscar.
     * @param album     Album del artista a buscar
     * @param size      Tamaño de la imagen solicitada
     * @return URL a la imagen del album.
     */
    public static String find(String artist, String album, ImageSize size)
    {
        //Check de las referencias iniciales
        if(artist == null || artist.isEmpty() || album == null || album.isEmpty() || size == null)
            return null;
        
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
                            //Consulto su nombre
                            if(child.getNodeName().equals(keyNAME))
                            {
                                //Si posee distinto nombre al que buscamos saltamos
                                if(!child.getFirstChild().getNodeValue().toLowerCase().equals(album.toLowerCase()))
                                    break;
                            }
                            //En caso de ser una imagen nos quedamos con la que precisemos
                            else if(child.getNodeName().equals(keyIMAGE))
                            {
                                //Switch en funcion del estado de la imagen requerida
                                switch(size)
                                {
                                    case SMALL:
                                        if(child.getAttributes().item(0).getNodeValue().equals(keySMALL))
                                        {
                                            return child.getFirstChild().getNodeValue();
                                        }                                        
                                        break;
                                    case MEDIUM:
                                        if(child.getAttributes().item(0).getNodeValue().equals(keyMEDIUM))
                                        {
                                            return child.getFirstChild().getNodeValue();
                                        }                                          
                                        break;
                                    case LARGE:
                                        if(child.getAttributes().item(0).getNodeValue().equals(keyLARGE))
                                        {
                                            return child.getFirstChild().getNodeValue();
                                        }                                             
                                        break;
                                    case EXTRALARGE:
                                        if(child.getAttributes().item(0).getNodeValue().equals(keyEXTRALARGE))
                                        {
                                            return child.getFirstChild().getNodeValue();
                                        }                                         
                                        break;                                        
                                }
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
        //Si llegamos aca es porque se produjo algun tipo de error desconocido
        return null;
    }
}
