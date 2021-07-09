package View;

import Model.Pair;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;

public class QueriesView {

    // all show queries algorithms

    public static void show_query_1_5(List<String> q1, String time,int inicio, int fim,int querie){
        View.headers(1);
        for(int i = inicio;i < fim; i++) {
            System.out.println("-------------------------");
            System.out.println(q1.get(i));

        }
        System.out.println("-------------------------");
        if(querie == 1) System.out.println("Número de negócios não avaliados: " + q1.size());
        else if(querie == 5) System.out.println("Número de businesses diferentes:" + q1.size());
        System.out.println(time);
    }

    public static void show_query_2(ArrayList<Integer> q2, String time){
        System.out.println("Número de reviews: " + q2.get(0));
        System.out.println("Número de utilizadores diferentes: " + q2.get(1));
        System.out.println(time);
    }

    public static void show_query_3(String q3, String time){
        System.out.println(q3);
        System.out.println(time);
    }

    public static void show_query_4(String q4, String time){
        System.out.println(q4);
        System.out.println(time);
    }

    public static void show_query_6(List<Pair<Integer,List<Pair<String, Pair<Integer,Integer>>>>> q6, String time,int inicio,int fim){
        for (int i = inicio; i < fim; i++){
            System.out.println(q6.get(i).getFirst());
            System.out.println("-------------------------");
            for (Pair<String, Pair<Integer,Integer>> top : q6.get(i).getSecond()){
                System.out.println(top.getFirst() + " " +
                        top.getSecond().getFirst() + " " + top.getSecond().getSecond());
            }
            System.out.println("-------------------------");
        }

        System.out.println(time);
    }

    public static void show_query_7(HashMap<String,List<Pair<Integer,String>>> q7, String time,boolean all,String cidade) {
        if (all) {
            for (Map.Entry<String, List<Pair<Integer, String>>> entry : q7.entrySet()) {
                System.out.println(entry.getKey());
                for (Pair<Integer, String> pair : entry.getValue()) {
                    System.out.println(pair.getSecond() + " " + pair.getFirst());
                }
                System.out.println("-------------------------");
            }
        } else {
            List<Pair<Integer, String>> city = q7.get(cidade);
            if (city == null) View.printError();
            else {
                System.out.println(cidade);
                for (Pair<Integer, String> pair : city) {
                    System.out.println(pair.getSecond() + " " + pair.getFirst());
                }
                System.out.println("-------------------------");
            }
        }
        System.out.println(time);
    }

    public static void show_query_8(List<Pair<Integer,String>> q8, String time,int inicio, int fim){
        View.headers(1);
        for(int i = inicio; i < fim; i++){
            System.out.println(q8.get(i).getSecond() + " -> " + q8.get(i).getFirst());
        }
        System.out.println(time);
    }

    public static void show_query_9(List<Pair<String,Pair<Integer,Float>>> q9, String time, int inicio, int fim){
        for(int i = inicio; i < fim; i++){
            System.out.println(q9.get(i).getFirst() + " -> " + q9.get(i).getSecond().getFirst()
                    + " -> " + q9.get(i).getSecond().getSecond());
        }
        System.out.println(time);
    }

    public static void show_query_10(Map<String,Map<String,List<Pair<String,Float>>>> q10, String time,boolean all,String cidade,String estado) {
        if (all) {
            for (Map.Entry<String, Map<String, List<Pair<String, Float>>>> state : q10.entrySet()) {
                System.out.println("State -> " + state.getKey());
                for (Map.Entry<String, List<Pair<String, Float>>> city : state.getValue().entrySet()) {
                    System.out.println("  City -> " + city.getKey());
                    for (Pair<String, Float> businesses : city.getValue()) {
                        System.out.println("    " + businesses.getFirst() + " " + businesses.getSecond());
                    }
                }
            }
        } else {
            Map<String, List<Pair<String, Float>>> state = q10.get(estado);
            if (state != null) {
                System.out.println("State -> " + estado);

                List<Pair<String, Float>> city = state.get(cidade);
                if (city != null) {
                    System.out.println("  City -> " + cidade);
                    for (Pair<String, Float> businesses : city) {
                        System.out.println("    " + businesses.getFirst() + " " + businesses.getSecond());
                    }
                    System.out.println(time);

                } else View.printError();
            } else View.printError();
        }
        System.out.println(time);
    }

}
