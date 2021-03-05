/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package java_version;

/**
 *
 * @author luis
 */
public class Java_version {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        System.out.println("System java version: " + System.getProperty("java.version"));
        System.out.println("System java vendor: " + System.getProperty("java.vendor"));
        System.out.println("System classpath: " + System.getProperty("java.class.path"));
        // TODO code application logic here
    }

}
