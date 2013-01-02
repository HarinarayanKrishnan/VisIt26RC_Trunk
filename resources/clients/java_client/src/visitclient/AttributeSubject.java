/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package visitclient;

import java.util.ArrayList;
import com.google.gson.JsonObject;

/**
 *
 * @author hari
 */
public class AttributeSubject
{
    public interface AttributeSubjectCallback
    {
        public void update(AttributeSubject subject);
    };
        
    public AttributeSubject()
    {
        callbackList = new ArrayList<AttributeSubjectCallback>();
    }

    public boolean update(JsonObject jo)
    {
        if(jo.has("api"))
            api = jo;
        else
        {
            data = jo;
            // tell all listeners object has been updated..
            for(AttributeSubjectCallback cb : callbackList) cb.update(this);
        }
        return true;
    }

    public JsonObject getData()
    {
        return data.getAsJsonObject("contents").getAsJsonObject("data");
    }
    
    public JsonObject getApi()
    {
        return api.getAsJsonObject("api").getAsJsonObject("data");
    }
    
    public void SetCallback(AttributeSubjectCallback callback)
    {
        callbackList.add(callback);
    }
    
    
    public void RemoveCallback(AttributeSubjectCallback callback)
    {
        callbackList.remove(callback);
    }
    
    public JsonObject data;
    public JsonObject api;
    public ArrayList<AttributeSubjectCallback> callbackList;
}
    
