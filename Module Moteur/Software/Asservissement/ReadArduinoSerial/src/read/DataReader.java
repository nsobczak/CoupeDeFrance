package read;

import java.io.*;

import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

import java.nio.charset.StandardCharsets;
import java.nio.file.FileAlreadyExistsException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Enumeration;


//______________________________________________________________________

/**
 * Created by ISEN on 31/01/2017.
 * DataReader : reading of data received on the serial port from the arduino
 */
public class DataReader implements SerialPortEventListener {
    SerialPort serialPort;
    private Path outputFile;
    private BufferedWriter bufferedWriter;

    public DataReader(Path outputFile) {
        this.outputFile = outputFile;
    }

    public void setBufferedWriter(BufferedWriter bufferedWriter) {
        this.bufferedWriter = bufferedWriter;
    }

    public Path getOutputFile() {
        return outputFile;
    }

    /**
     * The port we're normally going to use.
     */
    private static final String PORT_NAMES[] = {
            "/dev/tty.usbserial-A9007UX1", // Mac OS X
            "/dev/ttyACM0", // Raspberry Pi
            "/dev/ttyACM1",
            "/dev/ttyACM2",
            "/dev/ttyUSB0", // Linux
            "/dev/ttyUSB1",
            "/dev/ttyUSB2",
            "COM1", "COM2",
            "COM3", "COM4",
            "COM5", "COM6",
            "COM7", "COM8",
            "COM9", "COM10",
            "COM11", "COM12",
            "COM13", "COM14",// Windows
    };

    /**
     * A BufferedReader which will be fed by a InputStreamReader
     * converting the bytes into characters
     * making the displayed results codepage independent
     */
    private BufferedReader input;

    /**
     * The output stream to the port
     */
    private OutputStream output;

    /**
     * Milliseconds to block while waiting for port open
     */
    private static final int TIME_OUT = 3000;

    /**
     * Default bits per second for COM port.
     */
    private static final int DATA_RATE = 115200;


    //______________________________________________________________________

    public void initialize() {
        // the next line is for Raspberry Pi and
        // gets us into the while loop and was suggested here was suggested http://www.raspberrypi.org/phpBB3/viewtopic.php?f=81&t=32186
        // System.setProperty("gnu.io.rxtx.SerialPorts", "/dev/ttyACM0");

        CommPortIdentifier portId = null;
        Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();

        //First, Find an instance of serial port as set in PORT_NAMES.
        while (portEnum.hasMoreElements()) {
            CommPortIdentifier currPortId = (CommPortIdentifier) portEnum.nextElement();
            for (String portName : PORT_NAMES) {
                if (currPortId.getName().equals(portName)) {
                    portId = currPortId;
                    System.out.println("portId found : " + portId);
                    break;
                }
            }
        }
        if (portId == null) {
            System.out.println("Could not find COM port.");
            return;
        }

        try {
            // open serial port, and use class name for the appName.
            serialPort = (SerialPort) portId.open(this.getClass().getName(),
                    TIME_OUT);

            // set port parameters
            serialPort.setSerialPortParams(DATA_RATE,
                    SerialPort.DATABITS_8,
                    SerialPort.STOPBITS_1,
                    SerialPort.PARITY_NONE);

            // open the streams
            input = new BufferedReader(new InputStreamReader(serialPort.getInputStream()));
            output = serialPort.getOutputStream();
            System.out.println(output);

            // add event listeners
            serialPort.addEventListener(this);
            serialPort.notifyOnDataAvailable(true);
        } catch (Exception e) {
            System.err.println(e.toString());
        }
    }


    /**
     * This should be called when you stop using the port.
     * This will prevent port locking on platforms like Linux.
     */
    public synchronized void close() {
        if (serialPort != null) {
            serialPort.removeEventListener();
            serialPort.close();
        }
    }


    /**
     * Handle an event on the serial port. Read the data and print it.
     *
     * @param oEvent
     */
    public synchronized void serialEvent(SerialPortEvent oEvent) {
        if (oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
            try {
                //1st value
                String inputLine = input.readLine();
                System.out.println("===values===\n" + inputLine);
                writeFile(this.outputFile, this.bufferedWriter, ";" +  inputLine); // \n sous linux ; \r\n sous windows
                //2nd value
                inputLine = input.readLine();

                System.out.println(inputLine);
                writeFile(this.outputFile, this.bufferedWriter, ";" +  inputLine);
                //3rd value
                inputLine = input.readLine();
                System.out.println(inputLine);
                writeFile(this.outputFile, this.bufferedWriter, ";" +  inputLine);
                //4th value
                inputLine = input.readLine();
                System.out.println(inputLine);
                writeFile(this.outputFile, this.bufferedWriter, ";" +  inputLine + "\r\n");
            } catch (Exception e) {
                System.err.println(e.toString());
            }
        }
        // Ignore all the other eventTypes, but you should consider the other ones.
    }

    //______________________________________________________________________
    public void writeFile(Path pathToFilename, Writer writer, String toWrite) throws IOException {
        if (Files.notExists(pathToFilename)) {
            System.out.println("Fichier inexistant => création du fichier");
            Files.createFile(pathToFilename);
        }
        try {
            writer.write(toWrite);
            writer.flush();
        } catch (IOException e) {
            System.out.println("Write exception");
            e.printStackTrace();
        }
    }

    //______________________________________________________________________

    public static void main(String[] args) throws Exception {

        DataReader main = new DataReader(Paths.get("src","tableau.csv"));
        main.setBufferedWriter(Files.newBufferedWriter(main.getOutputFile(), StandardCharsets.UTF_8));
        main.initialize();

        Thread thread = new Thread() {
            public void run() {
                //the following line will keep this send alive for 1000 seconds,
                //waiting for events to occur and responding to them (printing incoming messages to console).
                try {
                    Thread.sleep(1000000000);
                } catch (InterruptedException ie) {
                }
            }
        };
        thread.start();
        System.out.println("Started");
    }


}
