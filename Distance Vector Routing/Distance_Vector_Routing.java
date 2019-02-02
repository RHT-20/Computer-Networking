import java.io.*;
import java.util.*;
import java.net.*;
import static com.sun.corba.se.impl.util.Utility.printStackTrace;
import java.nio.charset.StandardCharsets;
import java.time.*;

public class Distance_Vector_Routing
{
    public static void main(String[] args) throws Exception
    {
        init(args);
        
        Send sd = new Send();
        Receive rv = new Receive(adj_class.port_no);
        sd.start();
        rv.start();
    }
    
    static void init(String[] args) throws Exception
    {
        if(args.length<3)
        {
            System.out.println("Something missing.");
            System.exit(0);
        }
        new adj_class();
        
        String file_name,temp_str;
        boolean flag;
        
        adj_class.node_id = args[0].charAt(0);
        file_name = args[1];
        adj_class.port_no = Integer.parseInt(args[2]);
        
        FileReader fr = new FileReader(file_name);
        Scanner fscan = new Scanner(fr);
        
        flag = false;
        while(fscan.hasNext())
        {
            temp_str = fscan.nextLine();
            if(flag)
            {
                String[] s = temp_str.split(" ");
                
                edge ed = new edge(s[0].charAt(0), Double.parseDouble(s[1]), Integer.parseInt(s[2]));
                adj_class.adj.add(ed);
                
                int x = adj_class.node_id-'A'+1;
                int y = s[0].charAt(0)-'A'+1;
                double w = Double.parseDouble(s[1]);
                adj_class.dist[x][y] = w;
                adj_class.next_hop[x][y] = s[0].charAt(0);
            }
            else
            {
                adj_class.neighbor_no = Integer.parseInt(temp_str);
                flag = true;
            }
        }
        return;
    }
}

class edge 
{
    char node_id;
    double cost;
    int port_no;
    
    public edge(char nd,double c,int p)
    {
        this.node_id = nd;
        this.cost = c;
        this.port_no = p;
    }
}

class adj_class 
{
    public static ArrayList<edge>adj;
    public static char node_id;
    public static int port_no,neighbor_no;
    public static double[][] dist;
    public static char[][] next_hop;
    public static double INF;
    public static int[] cnt;
    public static boolean stable_flag;
    
    public adj_class()
    {
        adj = new ArrayList<>();
        next_hop = new char[35][];
        dist = new double[35][];
        cnt = new int[35];
        INF = 1000000;
        stable_flag = false;
        
        for(int i=0; i<35; i++)
        {
            dist[i] = new double[35];
            next_hop[i] = new char[35];
            cnt[i] = 0;
        }
        for(int i=0; i<35; i++)
        {
            for(int j=0; j<35; j++)
            {
                if(i==j)    dist[i][j] = 0;
                else    dist[i][j] = INF;
                next_hop[i][j] = '-';
            }
        }
    }
}

class Entry 
{
    private int x,y;
    private double cost;
    private char next_hop;

    public Entry(int x,int y,double c,char ch)
    {
        this.x = x;
        this.y = y;
        this.cost = c;
        this.next_hop = ch;
    }
}

class Send extends Thread
{
    DatagramSocket DS;
    String str;
    byte[] b;

    public Send() throws Exception
    {
        this.DS = new DatagramSocket();
        this.str = "";
        this.b = new byte[10000];
    }
    
    public void run()
    {
        try 
        {
            while(true)
            {
                sleep(5000);
                str = create_str();
                b = str.getBytes();
        
                InetAddress IA = InetAddress.getLocalHost();
                for(int i=0; i<adj_class.adj.size(); i++)
                {
                    int port = adj_class.adj.get(i).port_no;
                    DatagramPacket DP = new DatagramPacket(b, b.length, IA, port);
                    DS.send(DP);
                }
            }
        }
        catch (Exception e) 
        {
            printStackTrace();
        }
    }
    
    public String create_str()
    {
        String s = "";
        int x = adj_class.node_id-'A'+1;
        double w;
        
        for(int i=1; i<=10; i++)
        {
            if(x == i)  continue;
            w = adj_class.dist[x][i];
            if(w != adj_class.INF)
            {
                s += Integer.valueOf(x);
                s += " ";
                s += Integer.valueOf(i);
                s += " ";
                s += Double.valueOf(w);
                s += " ";
                s += Character.valueOf(adj_class.next_hop[x][i]);
                s += "*";
            }
        }
        if(s.length()>0)  s = s.substring(0,s.length()-1);
        return s;
    }
}

class Receive extends Thread
{
    DatagramSocket DS;
    byte[] b;
    String str;
        
    public Receive(int port) throws Exception
    {
        this.DS = new DatagramSocket(port);
        this.b = new byte[10000];
        this.str = "";
    }
        
    public void run()
    {
        try 
        {
            while(true)
            {
                DatagramPacket DP = new DatagramPacket(b, b.length);
                DS.receive(DP);

                str = new String(b, StandardCharsets.UTF_8);
                
                process();
                if(check() && adj_class.stable_flag==false)
                {
                    adj_class.stable_flag = true;
                    print();
                }
            }
        }
        catch (Exception e) 
        {
            printStackTrace();
        }
    }
    
    public synchronized boolean check()
    {
        int x = adj_class.node_id-'A'+1;
        if(adj_class.cnt[x]<10) return false;
        
        for(int i=0; i<adj_class.adj.size(); i++)
        {
            x = adj_class.adj.get(i).node_id-'A'+1;
            if(adj_class.cnt[x]<10) return false;
        }
        return true;
    }
    
    public synchronized void process()
    {
        boolean f = false;
        int pos = 0;
        String[] s1 = str.split("\\*");
        
        for(int i=0; i<s1.length; i++)
        {
            String[] s2 = s1[i].split("\\ ");
            int x = Integer.parseInt(s2[0]);
            pos = x;
            int y = Integer.parseInt(s2[1]);
            double w = Double.parseDouble(s2[2]);
            char hp = s2[3].charAt(0);
            
            if(adj_class.dist[x][y]>w)  f = true;
            
            adj_class.dist[x][y] = w;
            adj_class.next_hop[x][y] = hp;
        }
        
        if(f)   adj_class.cnt[pos] = 0;
        else    adj_class.cnt[pos]++;
        Relax();
        return;
    }
    
    public synchronized void Relax()
    {
        boolean f = false;
        int x = adj_class.node_id-'A'+1;
        for(int i=1; i<=10; i++)
        {
            if(adj_class.dist[x][i] != adj_class.INF)
            {
                for(int j=1; j<=10; j++)
                {
                    if(adj_class.dist[i][j] != adj_class.INF)
                    {
                        double w1 = adj_class.dist[x][i];
                        double w2 = adj_class.dist[i][j];
                        if((w1+w2) < adj_class.dist[x][j])
                        {
                            adj_class.dist[x][j] = w1+w2;
                            int ti = i+'A'-1;
                            adj_class.next_hop[x][j] = (char)ti;
                            f = true;
                        }
                    }
                }
            }
        }
        if(f)   adj_class.cnt[adj_class.node_id-'A'+1] = 0;
        else    adj_class.cnt[adj_class.node_id-'A'+1]++;
        
        return;
    }
    
    public void print()
    {
        int x = adj_class.node_id-'A'+1;
        String s1 = Character.toString(adj_class.node_id);
        System.out.println("Node "+adj_class.node_id+":");
        for(int i=1; i<=10; i++)
        {
            if(adj_class.dist[x][i] != adj_class.INF && x != i)
            {
                int y = i+'A'-1;
                String s2 = "";
                char ch = (char)y;
                s2 = Integer.toString(y);
                String s3 = Character.toString(adj_class.next_hop[x][i]);
                System.out.println("shortest path to node " + ch + " : " + "the next hop is "+ s3 + " and the cost is " + adj_class.dist[x][i]);
            }
        }
    }
}

