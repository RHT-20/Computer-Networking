package udp_socket_programming;

import com.sun.xml.internal.bind.api.impl.NameConverter;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.nio.charset.StandardCharsets;

public class UDP_Client 
{
    public static void main(String[] args) throws Exception
    {
        DatagramSocket DS = new DatagramSocket();
        
        String str = "Hello";
        byte[] b1 = new byte[1024];
        b1 = str.getBytes();
        //System.out.println(str+" "+b1);
        
        InetAddress IA = InetAddress.getLocalHost();
        DatagramPacket DP1 = new DatagramPacket(b1, b1.length, IA, 8888);    /// message in byte format, message length, receiver IP address, receiver port number
        
        DS.send(DP1);    /// send data
        
        byte[] b2 = new byte[1024];
        DatagramPacket DP2 = new DatagramPacket(b2, b2.length);
        DS.receive(DP2);
        
        str = new String(b2, StandardCharsets.UTF_8);
        System.out.println(str);
    }
}
