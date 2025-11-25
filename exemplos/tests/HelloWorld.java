public class HelloWorld {

    public static void main(String[] args) {
        int c;
        c = 0;
        for (int i = 0; i < 5; i++) {
            c = i;
            c++;
            if (c == 3) {
                break;
            }
        }   
    }
}
