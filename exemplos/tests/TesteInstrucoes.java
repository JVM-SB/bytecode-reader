public class TesteInstrucoes {

    public static void main(String[] args) {
        // 1. Teste astore (Armazenamento de Referência de Objeto)
        // Isso deve gerar astore_1 ou astore
        //Object obj = new Object();
        //System.out.println("Teste astore: Objeto criado e armazenado.");

        // 2. Teste fastore (Armazenamento de float em Array)
        // Isso deve gerar fastore
        float[] fArray = new float[1];
        fArray[0] = 10.5f;
        System.out.println("Teste fastore: " + fArray[0]);

        // 3. Teste lastore (Armazenamento de long em Array)
        // Isso deve gerar lastore
        long[] lArray = new long[1];
        lArray[0] = 9876543210L;
        System.out.println("Teste lastore: " + lArray[0]);

        // 4. Teste iaload (Carregamento de int de Array)
        // Isso deve gerar iastore e iaload
        int[] iArray = new int[2];
        iArray[0] = 42; // iastore
        int valorLido = iArray[0]; // iaload
        System.out.println("Teste iaload: " + valorLido);
    }
}
