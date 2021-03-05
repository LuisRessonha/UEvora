package so2;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class EchoServer {

    private int serverPort;

    public EchoServer(int p) {
        serverPort = p;
    }

    public static void main(String[] args) throws IOException {
        System.err.println("SERVER...");
        if (args.length < 1) {
            System.err.println("Missing parameter: port number");
            System.exit(1);
        }
        int p = 0;
        try {
            p = Integer.parseInt(args[0]);
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(2);
        }

        EchoServer serv = new EchoServer(p);
        serv.servico();   // activa o servico
    }

    // activa o servidor no porto indicado em "serverPort"
    public void servico() throws IOException {

        // exercicio 2: inicializar um socket para aceitar ligacoes...
        ServerSocket s = new ServerSocket(serverPort);

        while (true) {
            // ler a mensagem do client        
            Socket connectS = s.accept();

            InputStream socketIn = connectS.getInputStream();
            byte[] b = new byte[256];
            int lidos = socketIn.read(b);
            String resp = new String(b, 0, lidos);
            System.out.println("Resposta: " + resp);

            // escrever a resposta
            OutputStream socketOut = connectS.getOutputStream();
            socketOut.write(resp.getBytes());

            System.out.println(resp);

        }

    }

}
