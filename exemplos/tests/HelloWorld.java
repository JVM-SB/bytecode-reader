public class HelloWorld {

    public static double add(double a, double b) {
        return a + b;
    }
    
    public static int add(int a, int b) {
        return a + b;
    }

    public static long add(long a, long b) {
        return a + b;
    }
    public static void main(String[] args) {
        System.out.println(add(5, 5));
        System.out.println(add(5.5, 5.5));
        System.out.println(add(10L, 20L));
    }
}
