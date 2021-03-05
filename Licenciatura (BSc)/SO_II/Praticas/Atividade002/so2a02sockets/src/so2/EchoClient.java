package so2;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.io.InputStream;

public class EchoClient {

    private String address = null;
    private int sPort = 0;

    public EchoClient(String add, int p) {
        address = add;
        sPort = p;
    }

    public static void main(String[] args) {
        // exigir os argumentos necessarios
        if (args.length < 3) {
            System.err.println("Argumentos insuficientes:  java EchoClient ADDRESS PORT");
            System.exit(1);
        }

        try {
            String addr = args[0];
            int p = Integer.parseInt(args[1]);

            EchoClient cl = new EchoClient(addr, p);

            // ler o texto a enviar ao servidor
            String s = args[2];

            cl.go(s);   // faz o pretendido
        } catch (Exception e) {
            System.out.println("Problema no formato dos argumentos");
            e.printStackTrace();
        }
    }

    public void go(String msg) throws IOException {

        // exercicio 1: mostrar a mensagem que vai ser enviada
        // ...
        // ... exercicio 3
        Socket s = new Socket(address, sPort);
        // ja esta connected

        // escrever a mensagem
        OutputStream socketOut = s.getOutputStream();
        socketOut.write(msg.getBytes());

        // ler a resposta do servidor        
        InputStream socketIn = s.getInputStream();
        byte[] b = new byte[256];
        int lidos = socketIn.read(b);
        String resp = new String(b, 0, lidos);
        System.out.println("Resposta: " + resp);

    }

}
