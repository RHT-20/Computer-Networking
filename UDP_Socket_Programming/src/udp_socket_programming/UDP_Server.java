package udp_socket_programming;

import com.sun.org.apache.bcel.internal.generic.DUP2;
import com.sun.xml.internal.bind.api.impl.NameConverter;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.nio.charset.StandardCharsets;

public class UDP_Server 
{
    public static void main(String[] args) throws Exception
    {
        DatagramSocket DS = new DatagramSocket(8888);
        
        byte[] b1 = new byte[1024];
        DatagramPacket DP1 = new DatagramPacket(b1, b1.length);
        DS.receive(DP1);
        
        String str = new String(b1, StandardCharsets.UTF_8);
        System.out.println(str);
        
        str = "Hi";
        byte[] b2 = str.getBytes();
        //System.out.println(b2);
        
        InetAddress IA = InetAddress.getLocalHost();
        DatagramPacket DP2 = new DatagramPacket(b2, b2.length, IA, DP1.getPort());    /// message in byte format, message length, receiver IP address, receiver port number
        DS.send(DP2);    /// send data
    }
}
