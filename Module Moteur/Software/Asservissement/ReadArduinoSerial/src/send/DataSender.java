package send;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;


//______________________________________________________________________

/**
 * Created by ISEN on 31/01/2017.
 * DataSender : sending of data to Mosquitto
 */
public class DataSender
{
    public static void main(String[] args)
    {
        SendValues("10");
    }


    public static MqttClient SendValues(String values)
    {
        String topic = "MQTT Examples";
        String content = "Message from MqttPublishSample. Sended values : " + values;
        int qos = 2;
        String broker = "tcp://iot.eclipse.org:1883";
        String clientId = "JavaSample";
        MemoryPersistence persistence = new MemoryPersistence();

        return send(topic, content, qos, broker, clientId, persistence);
    }


    public static MqttClient send(String topic, String content, int qos, String broker, String clientId, MemoryPersistence persistence)
    {
        try
        {
            MqttClient sampleClient = new MqttClient(broker, clientId, persistence);
            startSender(broker, sampleClient);
            System.out.println("Publishing message: " + content);
            MqttMessage message = new MqttMessage(content.getBytes());
            message.setQos(qos);
            sampleClient.publish(topic, message);
            System.out.println("Message published");
            return sampleClient;
        //  stopSender(sampleClient);
        } catch (MqttException me)
        {
            System.out.println("reason " + me.getReasonCode());
            System.out.println("msg " + me.getMessage());
            System.out.println("loc " + me.getLocalizedMessage());
            System.out.println("cause " + me.getCause());
            System.out.println("excep " + me);
            me.printStackTrace();
        }
        return null;
    }


    public static void startSender(String broker, MqttClient sampleClient) throws MqttException
    {
        MqttConnectOptions connOpts = new MqttConnectOptions();
        connOpts.setCleanSession(true);
        System.out.println("Connecting to broker: " + broker);
        sampleClient.connect(connOpts);
        System.out.println("Connected");
    }


    public static void stopSender(MqttClient sampleClient) throws MqttException
    {
        sampleClient.disconnect();
        System.out.println("Disconnected");
        System.exit(0);
    }


}
