package controllers;


import com.sun.media.jfxmedia.logging.Logger;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.List;
import java.util.concurrent.Callable;
import models.Configuration;
import models.Device;
import models.DeviceConfig;
import models.User;
import play.data.DynamicForm;
import play.data.Form;
import play.libs.Akka;
import play.libs.F.Function;
import play.libs.F.Promise;
import play.mvc.Controller;
import play.mvc.Result;
import static play.mvc.Results.ok;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author agustin
 */
public class Devices extends Controller
{
    public static Result devices()
    {
        Long userId = Long.valueOf(session().get("id"));
        
        return ok(views.html.devicesPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createDeviceList(userId)));
    }
      
    private static List<Device> createDeviceList(Long userId)
    {
        return Device.findByUserId(userId);
    }
    
    public static Result add()
    {
        return ok(views.html.deviceNewPanel.render(controllers.routes.Devices.devices(),
                                                controllers.routes.Application.logout()));
    }
    
    public static Result onAdd()
    {
        Long userId = Long.valueOf(session().get("id"));
        User user = User.findById(userId);
        
        DynamicForm requestData = Form.form().bindFromRequest();
        String serial = requestData.get("serial");
        if(serial == null || serial.isEmpty())
        {
            flash("error", "Error en el numero serial");
            return badRequest(views.html.devicesPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createDeviceList(userId)));
        }
        
        Device device = Device.findBySerial(serial);
        if(device == null)
        {
            device = new Device();
            device.setSerial(serial);
            device.setUser(user);        
        }
        else
        {
            flash("error", "Ya  existe un dispositivo con el mismo numero serial.");
            return badRequest(views.html.devicesPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createDeviceList(userId)));            
        }
        
        
        try
        {
            Device.create(device);
        }
        catch(Exception e)
        {
            e.printStackTrace();
            flash("error", "Error en la creacion del dispositivo");
            return internalServerError(views.html.devicesPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createDeviceList(userId)));            
        }
        
        return created(views.html.devicesPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createDeviceList(userId)));
    }
    
    public static Result delete(Long id)
    {
        Long userId = Long.valueOf(session().get("id"));        
        
        Device device = Device.get(id);
        
        try
        {
            Device.delete(device);
            flash("success", "Dispositivo eliminado.");
        }
        catch(Exception e)
        {
            Logger.logMsg(Logger.ERROR, e.getMessage());
            flash("error", "Dispositivo no eliminado.");
        }
        
        return ok(views.html.devicesPanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            createDeviceList(userId)));
    }  
    
    public static Result getConfig(Long deviceId)
    {
        return ok(views.html.configDevicePanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            Device.get(deviceId),
                                            controllers.routes.Devices.config(),
                                            null));
    }   
    
    public static Result config()
    {
        try
        {
            //Obtengo el formulario y lo bindeo a la clase
            Form<DeviceConfig> requestForm = Form.form(DeviceConfig.class).bindFromRequest();
            if(requestForm.hasErrors())
            {
                return badRequest("Error en el formulario: " + requestForm.errors().toString());
            }

            //Obtengo el formulario de la request
            final DeviceConfig deviceConfig = requestForm.get();

            //Cargo el puerto
            Configuration configuration = Configuration.findById(Long.valueOf(session().get("id")));
            deviceConfig.setPort(15000);

            //Cargo la IP del servidor
            deviceConfig.setServerIP(listAvailableInets().get(0).getHostAddress());        
            
            
            Promise<Result> resultResponse = Akka.future(new Callable<Result>()
                {
                    public Result call()
                    {
                        //Ejecuto el envio al servidor de la configuracion
                        boolean status = sendConfigurationToDevice(deviceConfig);
                        if(status == true)
                        {
                            return ok(views.html.configDevicePanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            Device.get(deviceConfig.getId()),
                                            controllers.routes.Devices.config(),
                                            "Configurancion exitosa."));                            
                        }
                        else
                        {
                            return ok(views.html.configDevicePanel.render(controllers.routes.Application.panel(),
                                            controllers.routes.Application.logout(),
                                            Device.get(deviceConfig.getId()),
                                            controllers.routes.Devices.config(),
                                            "Error configurando."));                             
                        }
                        
                    }
                });
            
            return async(resultResponse.map(new Function<Result, Result> ()
                {
                    public Result apply(Result result)
                    {
                        return result;
                    }
                }));
            
            //return ok(Json.toJson(deviceConfig));
        }
        catch(Exception e)
        {
            return internalServerError(e.getMessage());
        }
    }
    
    private static boolean sendConfigurationToDevice(DeviceConfig deviceConfig) 
    {
        //Serializamos el objeto
        byte[] byteBuffer = deviceConfig.toBytes();
        if(byteBuffer == null)
        {
            return false;
        }
        
        //Creamos un socket UDP
        DatagramSocket socket = null;
        try
        {
            //Creamos la direccion y el puerto
            InetAddress address = InetAddress.getByName("192.168.2.200");
            int port = 15001;
            //Creamos el socket
            socket = new DatagramSocket();
            //Creamo el datagrama
            DatagramPacket packet = new DatagramPacket( byteBuffer, byteBuffer.length, address, port ) ;
            //Enviamos el paquete
            socket.send(packet);
            //Seteamos un timeout de recepcion
            socket.setSoTimeout(5 * 1000);
            //Esperamos una respuesta del servidor
            socket.receive(packet);
            //Obtenemos la informacion
            byte [] response = packet.getData();
            //Evaluamos la respuesta
            if(response != null)
            {
                String strResponse;
                strResponse = new String(Arrays.copyOfRange(response, 0, (int)packet.getLength()));
                System.out.println(strResponse);
                if(strResponse.equals("ACK"))
                {
                    return true;
                }
            }
        }
        catch(Exception e)
        {
            System.out.println("controllers.Devices: " + e.getMessage());
        }
        finally
        {
            socket.close();
        }
        
        return false;
    }
    
    private static List<InetAddress> listAvailableInets()
    {
        List<InetAddress> adresses = new ArrayList<>();
        try 
        {
            Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();

            while(true)
            {
                try
                {
                    NetworkInterface interf = interfaces.nextElement();
                    Enumeration<InetAddress> inetAddresses = interf.getInetAddresses();
                    while(true)
                    {
                        try
                        {
                            InetAddress inetAddress = inetAddresses.nextElement();
                            if(!inetAddress.isLoopbackAddress() && !inetAddress.isAnyLocalAddress() && 
                               !inetAddress.isLinkLocalAddress() && inetAddress.getHostAddress().contains("."))
                            {
                                System.out.println(inetAddress);
                                adresses.add(inetAddress);
                            }
                        }
                        catch(Exception e)
                        {
                            break;
                        }
                    }
                }
                catch(Exception e)
                {
                    break;
                }
            }
        } 
        catch (Exception e) 
        {
            System.out.println(e.getMessage());
        }
        
        return adresses;
    }

    private static void DatagramSocket() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
}

