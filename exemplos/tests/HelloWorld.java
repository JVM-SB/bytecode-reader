public class HelloWorld {
    
    public static int add(int a, int b) {
        return a + b;
    }
    public static void main(String[] args) {
        int p = 5 + 10;
        long bigNumber = 123456789L;
        double decimalNumber = 5.5;
        float floatNumber = 3.4f;
        System.out.println(add(p, 5));
        System.out.println("Hello, World!");
        System.out.println(true);
        System.out.println(bigNumber);   
        System.out.println(decimalNumber);
        System.out.println(floatNumber);
    }
}
