/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package visitclient;

import com.google.gson.JsonObject;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import vtk.vtkActor;
import vtk.vtkNativeLibrary;
import vtk.vtkPanel;
import vtk.vtkGenericDataObjectReader;
import java.io.FileOutputStream;
import java.io.IOException;

import sun.misc.BASE64Decoder;

/**
 *
 * @author hari
 */
public class VisItRenderingClient extends JPanel implements ActionListener, 
                                                 AttributeSubject.AttributeSubjectCallback
{
    // -----------------------------------------------------------------
    // Load VTK library and print which library was not properly loaded
    static {
        if(System.getProperty("vtk.lib.dir") == null)
            System.setProperty("vtk.lib.dir", 
                    "/work/visit/trunk/test/visit-vtk-5.8.0.a-build/bin");
        if (!vtkNativeLibrary.LoadAllNativeLibraries()) {
            for (vtkNativeLibrary lib : vtkNativeLibrary.values()) {
                if (!lib.IsLoaded()) {
                    System.out.println(lib.GetLibraryName() + " not loaded");
                }
            }
        }
        vtkNativeLibrary.DisableOutputWindow(null);
    }
    
    private static final long serialVersionUID = 1L;
    private vtkPanel renWin;
    private JButton exitButton;
    private vtkActor datasetActor;
    
    // -----------------------------------------------------------------
    public VisItRenderingClient() 
    {
        super(new BorderLayout());

    
        datasetActor = new vtkActor();
        
        renWin = new vtkPanel();
        renWin.GetRenderer().AddActor(datasetActor);

        // Add Java UI components
        exitButton = new JButton("Exit");
        exitButton.addActionListener(this);

        add(renWin, BorderLayout.CENTER);
        add(exitButton, BorderLayout.SOUTH);
    
    }
    
    public void updateImage(String imagedata)
    {
        //System.out.println(imagedata);
        BASE64Decoder decoder = new BASE64Decoder();
        
        try
        {
            byte[] output = decoder.decodeBuffer(imagedata);

            System.out.println("Writing file abc.jpg" );
            FileOutputStream stream = new FileOutputStream("abc.jpg");
            stream.write(output);
            stream.close();
  
            vtk.vtkJPEGReader r = new vtk.vtkJPEGReader();
            r.SetFileName("abc.jpg");
           
//            vtkGenericDataObjectReader reader = new vtkGenericDataObjectReader();
//            reader.ReadFromInputStringOn();
//            reader.SetBinaryInputString(output,output.length);
//            reader.Update();
            
            vtk.vtkDataSetMapper mapper = new vtk.vtkDataSetMapper();
            //vtkImageMapper mapper = new vtkImageMapper();
            mapper.SetInput((vtk.vtkImageData)r.GetOutput());
                        
            datasetActor.SetMapper(mapper);
            ((vtk.vtkImageData)r.GetOutput()).GetDimensions();
            //renWin.GetRenderWindow().SetTileViewport(0, 0, , WIDTH);
            renWin.repaint();
        }
        catch(Exception e)
        {}
        
    }
    
    public void updateVTK(String vtkdata)
    {
        //System.out.println(imagedata);
        BASE64Decoder decoder = new BASE64Decoder();
        
        try
        {
            byte[] output = decoder.decodeBuffer(vtkdata);
            
            vtkGenericDataObjectReader reader = new vtkGenericDataObjectReader();

            reader.ReadFromInputStringOn();
            reader.SetBinaryInputString(output,output.length);
            reader.Update();
            
//            vtk.vtkDataSetWriter writer = new vtk.vtkDataSetWriter();
//            writer.SetInput(reader.GetOutput());    
//            writer.SetFileName("abc.vtk");
//            writer.Write();
            
            vtk.vtkDataSetMapper mapper = new vtk.vtkDataSetMapper();
            
            mapper.SetInput((vtk.vtkDataSet)reader.GetOutput());
            mapper.Update();
            
            datasetActor.SetMapper(mapper);
            
            renWin.repaint();
        }
        catch(Exception e)
        {}
    }

    /** An ActionListener that listens to the button. */
    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource().equals(exitButton)) {
            System.exit(0);
        }
    }

    @Override
    synchronized public void update(AttributeSubject subject) 
    {
        //System.out.println("Called " + subject);
        JsonObject jo = subject.getData();
        
        String resultsMessage = jo.get("0").getAsString();
        
        if(resultsMessage.equals("ImageData"))
        {
            String defaultVars = jo.get("12").getAsJsonArray().get(0).getAsString();
            updateImage(defaultVars);
        }
        
        if(resultsMessage.equals("AVTData"))
        {
            int imageType = jo.get("3").getAsJsonArray().get(0).getAsInt();
            String defaultVars = jo.get("12").getAsJsonArray().get(0).getAsString();
            if(imageType == 1)
                updateImage(defaultVars);
            else if(imageType == 2)
                updateVTK(defaultVars);
        }        
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String [] args) throws InterruptedException, IOException
    {
        System.out.println("Starting Java Client");
        final VisItRenderingClient renderclient = new VisItRenderingClient();
        
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                JFrame frame = new JFrame("VisItRenderingClient");
                frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                frame.getContentPane().setLayout(new BorderLayout());
                frame.getContentPane().add(renderclient, BorderLayout.CENTER);
                frame.setSize(400, 400);
                frame.setLocationRelativeTo(null);
                frame.setVisible(true);
            }
        });
        
        VisItProxy client = new VisItProxy();
        
        if(!client.connect("localhost",9002,"bob"))
        {
            System.out.println("Could not connect to VisIt, Quitting");
            System.exit(0);
        }
        
        client.GetViewerState().get(38).SetCallback(renderclient);

        String CurLine = ""; // Line read from standard in
        
        System.out.println("Enter a line of text (type 'quit' to exit): ");
        InputStreamReader converter = new InputStreamReader(System.in);
        BufferedReader in = new BufferedReader(converter);
  
        ViewerMethods methods = client.GetViewerMethods();
        
        while (!(CurLine.equals("quit()")))
        {
            System.out.print(">> ");
            CurLine = in.readLine().trim();
              
            try{
                if ((CurLine.equals("quit()")))
                    continue;
                
                if("InvertBackgroundColor()".equals(CurLine))
                    methods.InvertBackgroundColor();

                if("AddWindow()".equals(CurLine))
                    methods.AddWindow();

                if("DrawPlots()".equals(CurLine))
                    methods.DrawPlots();

                if(CurLine.startsWith("OpenDatabase"))
                {
                    String ss = CurLine.substring(CurLine.indexOf("\"")+1,CurLine.lastIndexOf("\""));
                    methods.OpenDatabase(ss);
                }
                if(CurLine.startsWith("AddPlot"))
                {
                    String is = CurLine.substring(CurLine.indexOf("(")+1,CurLine.indexOf(")"));
                    is = is.replace("\"", "");

                    String [] results = is.split(",");
                    if(results.length == 2)
                        methods.AddPlot(results[0], results[1]);
                }

                if(CurLine.startsWith("AddOperator"))
                {
                    String is = CurLine.substring(CurLine.indexOf("(")+1,CurLine.indexOf(")"));
                    is = is.replace("\"", "");
                    methods.AddOperator(is);
                }

                if(CurLine.startsWith("DeleteActivePlots()"))
                    methods.DeleteActivePlots();

                if(CurLine.startsWith("HideActivePlots()"))
                    methods.HideActivePlots();

                if(CurLine.startsWith("SetActivePlots"))
                {
                    String is = CurLine.substring(CurLine.indexOf("(")+1,CurLine.indexOf(")"));
                    is = is.replace("\"", "");
                    methods.SetActivePlots(Integer.parseInt(is));
                }
            }
            catch(Exception e)
            {
                System.out.println("Error evaluating: " + CurLine);
            }
        }
        
        System.exit(0);
    }
}
