package controllers;


import models.Configuration;
import models.User;
import play.data.DynamicForm;
import play.data.Form;
import play.mvc.Controller;
import play.mvc.Result;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author agustin
 */
public class Configurations extends Controller
{
    public static Result configurations()
    {
        Long userId = new Long(session().get("id"));
        User user = User.findById(userId);
        
        Configuration configuration = Configuration.findByUserId(userId);
        
        return ok(views.html.configPanel.render(controllers.routes.Application.panel(),
                                             controllers.routes.Application.logout(),
                                             configuration,
                                             user));
    }
    
    public static Result update()
    {
        Long userId = new Long(session().get("id"));
        User user = User.findById(userId);
        Configuration configuration = Configuration.findByUserId(userId);
        
        try
        {
            DynamicForm formLogin = Form.form().bindFromRequest();
            
            //Cargo las configuraciones
            Integer udpPort = Integer.valueOf(formLogin.get("configuration.udpPort"));
            if(udpPort < 10000)
            {
                return badRequest(views.html.configPanel.render(controllers.routes.Application.panel(),
                                             controllers.routes.Application.logout(),
                                             configuration,
                                             user));
            }
            configuration.setUdpPort(udpPort);
            
            //Evaluo el usuario
            String email = formLogin.get("user.email");
            if(!(email == null || email.isEmpty() || user.getEmail().equals(email)))
            {
                user.setEmail(email);
            }
            
            String password = formLogin.get("user.password");
            String repassword = formLogin.get("user.repassword");
            if(!(password == null || repassword == null || password.isEmpty() || repassword.isEmpty() || !password.equals(repassword)))
            {
                user.setPassword(password);
                user.setRepassword(repassword);
            }
            else if(password != null && repassword != null && !password.equals(repassword))
            {
                return badRequest(views.html.configPanel.render(controllers.routes.Application.panel(),
                                             controllers.routes.Application.logout(),
                                             configuration,
                                             user));            
            }
            
            //Actualizo las configuraciones y el usuario
            Configuration.update(configuration);
            
            User.update(user);
        }
        catch(Exception e)
        {
            e.printStackTrace();
            return internalServerError(views.html.configPanel.render(controllers.routes.Application.panel(),
                                         controllers.routes.Application.logout(),
                                         configuration,
                                         user));        
        }
        
        return ok(views.html.configPanel.render(controllers.routes.Application.panel(),
                        controllers.routes.Application.logout(),
                        configuration,
                        user));
    }
}
