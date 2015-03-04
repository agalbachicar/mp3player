package controllers;

import com.avaje.ebean.Ebean;
import com.player.app.RApplication;
import java.io.File;
import java.nio.file.Files;
import java.nio.file.attribute.FileAttribute;
import java.nio.file.attribute.PosixFilePermission;
import java.nio.file.attribute.PosixFilePermissions;
import java.util.Set;
import models.User;
import play.*;
import play.data.DynamicForm;
import play.data.Form;
import play.mvc.*;

import views.html.*;

public class Application extends Controller 
{
    
    public static Result login()
    {
        return ok(views.html.login.render(controllers.routes.Application.authenticate(),
                controllers.routes.Application.register()));
    }
    
    public static Result register()
    {
        try
        {
            DynamicForm formLogin = Form.form().bindFromRequest();
            
            //Creamos un usuario
            User user = new User();
            user.setEmail(formLogin.get("email"));
            user.setPassword(formLogin.get("password"));
            if(user.isValidUser() && User.findByEmail(user.getEmail()) == null)
            {
                Ebean.beginTransaction();
                try
                {
                    //Creo el usuario
                    user.setName("Nombre");
                    user.setSurname("Apellido");
                    user.setRepassword(user.getPassword());
                    User.create(user);
                    
                    //Creamos una configuracion
                    models.Configuration configuration = new models.Configuration();
                    configuration.setUdpPort(RApplication.SERVER_PORT);
                    configuration.setUser(user);
                    models.Configuration.create(configuration);
                    
                    Ebean.commitTransaction();
                }
                catch(Exception e)
                {
                    //Se produjo un error creando el usuario
                    Ebean.rollbackTransaction();
                    //Se produjo un error, redirigimos a la pantalla de login
                    flash("error", "Error creando usuario.");
                    return redirect(controllers.routes.Application.login());
                }
                //Creamos el directorio para el usuario
                File userDir = new File(System.getProperty("user.dir").replaceAll("\\\\", "/") + "/fs/" + user.getId());
                Set<PosixFilePermission> perms = PosixFilePermissions.fromString("rwxrwx--x");
                FileAttribute<Set<PosixFilePermission>> fileAttributes = PosixFilePermissions.asFileAttribute(perms);
                Files.createDirectories(userDir.toPath(), fileAttributes);
                
                //Vaciamos la cookie
                session().clear();
                //Seteamos el id de usuario
                session().put("id", user.getId().toString());
                
                //Redirigimos al panel
                return redirect(controllers.routes.Application.panel());
            }
            else
            {
                    //Se produjo un error, redirigimos a la pantalla de login
                    flash("error", "Error creando usuario.");
                    return redirect(controllers.routes.Application.login());            
            }
        }
        catch(Exception e)
        {
            //Se produjo un error, redirigimos a la pantalla de login
            flash("error", "Error creando usuario.");
            return redirect(controllers.routes.Application.login());
        }
    }
    
    public static Result authenticate ()
    {
        try
        {
            DynamicForm formLogin = Form.form().bindFromRequest();
        
            User user = User.isValid(formLogin.get("email"), formLogin.get("password"));
            
            if(user != null)
            {
                session().put("id", user.getId().toString());
            }
            else
            {
                session().clear();
                flash("error", "Usuario o contraseñas incorrectos. Vuelva a intentar.");
                return unauthorized(views.html.login.render(controllers.routes.Application.login(),
                    controllers.routes.Application.register()));                
            }
        }
        catch(Exception e)
        {
            flash("error", "Error autentificando. Vuelva a intentar");
            return internalServerError(views.html.login.render(controllers.routes.Application.authenticate(),
                controllers.routes.Application.register()));              
        }
        
        return redirect(controllers.routes.Application.panel());
    }
    
    public static Result logout()
    {
        session().clear();
        
        return redirect(controllers.routes.Application.login());
    }
    
    public static Result panel()
    {
        return ok(views.html.startPanel.render(null, controllers.routes.Application.logout()));
    }    
}
