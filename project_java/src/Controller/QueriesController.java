package Controller;

import Model.*;
import View.*;

import java.util.*;

public class QueriesController {


    public static void queriesController(GestReviews sgr) {
        Scanner user_input = new Scanner(System.in);
        int queriesMenuOption = 0;
        int x;
        while (queriesMenuOption != 11) {
            View.queriesMenu();
            View.questions(7);
            queriesMenuOption = Controller.readInt(user_input);

            switch (queriesMenuOption) {
                case 1 -> {
                    user_input.nextLine();
                    List<String> q1 = Queries.query_1(sgr);
                    QueriesController.query1_5Controller(q1, Crono.getTime(),1);
                }
                case 2 -> {
                    View.questions(1);
                    int mes = Controller.readInt(user_input);
                    View.questions(2);
                    int ano = Controller.readInt(user_input);
                    try {
                        ArrayList<Integer> q2 = Queries.query_2(sgr, mes, ano);
                        QueriesView.show_query_2(q2, Crono.getTime());
                    }catch (NotEnoughData | InvalidArgument e) {
                        View.print(e.getMessage());
                    }
                    user_input.nextLine();
                    View.questions(8);
                    user_input.nextLine();


                }
                case 3 -> {
                    View.questions(3);
                    user_input.nextLine();
                    String usr_id3 = user_input.nextLine();
                    try {
                        QueriesView.show_query_3(Queries.query_3(sgr, usr_id3), Crono.getTime());
                    }catch (IdNotFound e) {
                        View.print(e.getMessage());
                    }
                    user_input.nextLine();
                    View.questions(8);
                    user_input.nextLine();

                }
                case 4 -> {
                    View.questions(4);
                    user_input.nextLine();
                    String bus_id = user_input.nextLine();
                    try {
                        QueriesView.show_query_4(Queries.query_4(sgr, bus_id), Crono.getTime());
                    }catch (IdNotFound e) {
                        View.print(e.getMessage());
                    }
                    user_input.nextLine();
                    View.questions(8);
                    user_input.nextLine();


                }
                case 5 -> {
                    View.questions(3);
                    user_input.nextLine();
                    String usr_id5 = user_input.nextLine();
                    try {
                        List<String> q5 = Queries.query_5(sgr, usr_id5);
                        QueriesController.query1_5Controller(q5,Crono.getTime(),5);
                    }catch(IdNotFound e) {
                        View.print(e.getMessage());
                    }

                }
                case 6 -> {
                    View.questions(5);
                    x = Controller.readInt(user_input);
                    try {
                        List<Pair<Integer, List<Pair<String, Pair<Integer, Integer>>>>> q6 = Queries.query_6(sgr, x);
                        QueriesController.query6Controller(q6, Crono.getTime());
                    }catch (NotEnoughData e) {
                        View.print(e.getMessage());
                    }


                }
                case 7 -> QueriesController.query7Controller(Queries.query_7(sgr),Crono.getTime());

                case 8 -> {
                    View.questions(6);
                    x = Controller.readInt(user_input);
                    List<Pair<Integer, String>> q8 = Queries.query_8(sgr, x);
                    QueriesController.query8Controller(q8, Crono.getTime());

                }
                case 9 -> {
                    View.questions(4);
                    user_input.nextLine();
                    String usr_id9 = user_input.nextLine();
                    View.questions(6);
                    x = Controller.readInt(user_input);
                    List<Pair<String,Pair<Integer,Float>>> q9 = Queries.query_9(sgr, usr_id9, x);
                    QueriesController.query9Controller(q9, Crono.getTime());
                }
                case 10 -> {
                    Map<String, Map<String,List<Pair<String,Float>>>> q10 = Queries.query_10(sgr);
                    QueriesController.query10Controller(q10,Crono.getTime());
                }
            }
        }

    }


    //Sub Controllers

    /**Controller for query1 and 5 that gives pagination type view */
    public static void query1_5Controller(List<String> q1,String time,int querie) {
        Scanner user_input = new Scanner(System.in);
        int size = q1.size();
        String response;
        int inicio = 0;
        int fim = 20;
        if(fim>size) fim = size;
        QueriesView.show_query_1_5(q1, time, inicio, fim,querie);
        View.questions(9);
        label:
        while (true) {
            response = user_input.nextLine();
            switch (response) {
                case "quit":
                    break label;
                case "next":
                    inicio += 20;
                    fim += 20;
                    if(fim > size) {
                        inicio = fim-20;
                        fim = size;
                        if(inicio<0) inicio = 0;
                    }
                    QueriesView.show_query_1_5(q1, time, inicio, fim,querie);
                    View.questions(9);
                    break;
                case "previous":
                    inicio -= 20;
                    fim -= 20;
                    if (inicio <= 0) {
                        inicio = 0;
                        fim = 20;
                        if(fim > size) fim = size;
                    }
                    QueriesView.show_query_1_5(q1, time, inicio, fim,5);
                    View.questions(9);
                    break;
            }
        }
    }


    /**Controller for query 6 that gives pagination type view */
    public static void query6Controller(List<Pair<Integer, List<Pair<String, Pair<Integer, Integer>>>>> q6,String time) {
        Scanner user_input = new Scanner(System.in);
        int size = q6.size();
        String response;
        int inicio = 0;
        int fim = 20;
        if(fim > size) fim = size;
        QueriesView.show_query_6(q6, time, inicio, fim);
        View.questions(9);
        label:
        while (true) {
            response = user_input.nextLine();
            switch (response) {
                case "quit":
                    break label;
                case "next":
                    inicio += 20;
                    fim += 20;
                    if(fim > size) {
                        inicio = fim-20;
                        fim = size;
                        if(inicio <= 0) inicio = 0;
                    }
                    QueriesView.show_query_6(q6, time, inicio, fim);
                    View.questions(9);
                    break;
                case "previous":
                    inicio -= 20;
                    fim -= 20;
                    if (inicio <= 0) {
                        inicio = 0;
                        fim = 20;
                        if(fim > size) fim = size;
                    }
                    QueriesView.show_query_6(q6, time, inicio, fim);
                    View.questions(9);
                    break;
            }
        }
    }

    /**Controller for query 8 that gives pagination type view */
    public static void query8Controller(List<Pair<Integer,String>> q8,String time) {
        Scanner user_input = new Scanner(System.in);
        int size = q8.size();
        String response;
        int inicio = 0;
        int fim = 20;
        if(fim > size) fim = size;
        QueriesView.show_query_8(q8, time, inicio, fim);
        View.questions(9);
        label:
        while (true) {
            response = user_input.nextLine();
            switch (response) {
                case "quit":
                    break label;
                case "next":
                    inicio += 20;
                    fim += 20;
                    if(fim > size) {
                        inicio = fim-20;
                        System.out.println(inicio + "--" + fim);
                        fim = size;
                        if(inicio<0) inicio = 0;
                    }
                    QueriesView.show_query_8(q8, time, inicio, fim);
                    View.questions(9);
                    break;
                case "previous":
                    inicio -= 20;
                    fim -= 20;
                    System.out.println(inicio + "--" + fim);
                    if (inicio <= 0) {
                        inicio = 0;
                        fim = 20;
                        if(fim > size) fim = size;
                    }
                    QueriesView.show_query_8(q8, time, inicio, fim);
                    View.questions(9);
                    break;
            }
        }
    }

    /**Controller for query 7 that gives the user the choice to see all of the information or part of it */
    public static void query7Controller(HashMap<String,List<Pair<Integer,String>>> q7, String time) {
        Scanner user_input = new Scanner(System.in);
        String response = "";
        while(!response.equals("quit")) {
            View.questions(12);
            response = user_input.nextLine();
            if(response.equals("all")) QueriesView.show_query_7(q7,time,true,null);
            else if(response.equals("one")){
                View.questions(11);
                String city = user_input.nextLine();
                QueriesView.show_query_7(q7,time,false,city);
            }
        }
    }


    /**Controller for query 8 that gives pagination type view */
    public static void query9Controller(List<Pair<String,Pair<Integer,Float>>> q9,String time){
        Scanner user_input = new Scanner(System.in);
        int size = q9.size();
        String response;
        int inicio = 0;
        int fim = 20;
        if(fim > size) fim = size;
        QueriesView.show_query_9(q9, time, inicio, fim);
        View.questions(9);
        label:
        while (true) {
            response = user_input.nextLine();
            switch (response) {
                case "quit":
                    break label;
                case "next":
                    inicio += 20;
                    fim += 20;
                    if(fim > size) {
                        inicio = fim-20;
                        fim = size;
                        if(inicio<0) inicio = 0;
                    }
                    QueriesView.show_query_9(q9, time, inicio, fim);
                    View.questions(9);
                    break;
                case "previous":
                    inicio -= 20;
                    fim -= 20;
                    if (inicio <= 0) {
                        inicio = 0;
                        fim = 20;
                        if(fim > size) fim = size;
                    }
                    QueriesView.show_query_9(q9, time, inicio, fim);
                    View.questions(9);
                    break;
            }
        }
    }


    /**Controller for query 10 that gives the user the choice to see all of the information or part of it */
    public static void query10Controller(Map<String, Map<String,List<Pair<String,Float>>>> q10,String time) {
        Scanner user_input = new Scanner(System.in);
        String response = "";
        while(!response.equals("quit")) {
            View.questions(12);
            response = user_input.nextLine();
                if(response.equals("all")) QueriesView.show_query_10(q10,time,true,null,null);
                else if(response.equals("one")){
                    View.questions(10);
                    String state = user_input.nextLine();
                    View.questions(11);
                    String city = user_input.nextLine();
                    QueriesView.show_query_10(q10,time,false,city,state);
                }
        }
    }

}
