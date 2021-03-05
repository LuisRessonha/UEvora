// Maximo divisor comum

class aula1 {
     public static int mdc(int a, int b) {
          if (a == b)
               return a;
          else if (b > a)
               return mdc(a, b - a);

          return mdc(a - b, b);
     }

     public static void main(String[] args) {
          System.out.println(mdc(Integer.parseInt(args[0]), Integer.parseInt(args[1])));
     }

}
