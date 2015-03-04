
import com.player.app.RApplication;
import java.io.File;
import models.Configuration;
import models.User;
import play.Application;
import play.GlobalSettings;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author agustin
 */
public class Global extends GlobalSettings
{
    @Override
    public void onStart(Application aplctn) 
    {
        System.out.println("onStart");
        try
        {
            initUDPServer();        
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }

    }   
    
    /**
     * Apaga el servidor antes de cerrar la aplicacion para que no queden threads activos.
     * @param aplctn 
     */    
    @Override
    public void onStop(Application aplctn)
    {
        System.out.println("onStop");
        RApplication.stopServer();
    }
    
    /**
     * Inicializo el server UDP.
     */
    public static void initUDPServer()
    {   
        Configuration configuration = Configuration.findByUserId(1L);
        
        RApplication.populateCommandsHandlers();
    }
}
