package Controller;

import static java.lang.System.nanoTime;
/**
 * Controller.Crono module
 * */

public class Crono {
    private static long inicio = 0L;
    private static long fim = 0L;

    /**Time functions */
    public static void start() {
        inicio = nanoTime();
    }

    public static void stop() {
        fim = nanoTime();
    }

    public static String getTime() {
        long time = fim - inicio;
        double timeD = time / 1.0E09;
        return "Elapsed Time: " + timeD + " s\n";
    }
}
