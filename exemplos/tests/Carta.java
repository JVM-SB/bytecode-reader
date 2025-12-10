// Source code is decompiled from a .class file using FernFlower decompiler (from Intellij IDEA).
public class Carta {
   public int naipe;
   public int valor;

   public Carta(int var1, int var2) {
      this.naipe = var1;
      this.valor = var2;
   }

   public static void main(String[] args) {
      Carta var2 = new Carta(2, 13);
      if (var2.naipe == 0) {
         System.out.println("O Naipe da carta e: ouros");
      } else if (var2.naipe == 1) {
         System.out.println("O Naipe da carta e: paus");
      } else if (var2.naipe == 2) {
         System.out.println("O Naipe da carta e: copas");
      } else if (var2.naipe == 3) {
         System.out.println("O Naipe da carta e: espadas");
      }

      System.out.println("O Naipe da carta e: ");
      System.out.println(var2.naipe);
      System.out.println("\n");
      System.out.println("O valor da carta e: ");
      System.out.println(var2.valor);
      System.out.println("\n");
      System.out.println("O nome da carta e: ");
      System.out.println(var2.naipe == 0 ? "Ouros" : (var2.naipe == 1 ? "Paus" : (var2.naipe == 2 ? "Copas" : "Espadas")));
      System.out.println("\n");
   }
}
