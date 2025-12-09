public class ALoadAStoreTeste {
    public static void main(String[] args) {
        String s = "oi";
        Object o = s;     // aqui rola aload/astore
        System.out.println(o);
        System.out.println(s);
    }
}
