package Controller;

import Model.Stats;
import View.*;

import java.io.IOException;
import java.util.Scanner;




public class Controller {

    public static void Run() {
        //initializes all 
        GestReviews sgr = new GestReviews(); //All sgr needed
        Scanner user_input = new Scanner(System.in);
        int mainMenuOption;

        Controller.loadFilesController(sgr);

        Stats statistics = new Stats(sgr);


        while(true) {
            View.mainMenu();
            View.questions(7);
            mainMenuOption = readInt(user_input);

            switch (mainMenuOption) {
                case 1 -> {                                                 //statistics
                    View.print(statistics.toString());
                    user_input.nextLine();
                    View.questions(8);
                    user_input.nextLine();
                }
                case 2 -> Controller.statisticsController(statistics);
                case 3 ->                                                         //controls all queries
                        QueriesController.queriesController(sgr);
                case 4 ->                                                         //loads new sgr
                        Controller.loadFilesController(sgr);
                case 5 -> {
                    try {                                                       //saves binary sgr
                        sgr.saveBinary("db/gestReviews.dat");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    View.print("Saving Files (.dat): " + Crono.getTime());
                }
                case 6 -> {                                                         //loads binary sgr
                    try {
                        sgr = new GestReviews("db/gestReviews.dat");
                    } catch (IOException | ClassNotFoundException e) {
                        e.printStackTrace();
                    }
                    View.print("Loading Files (.dat): " + Crono.getTime());
                }
                case 7 -> {                                                         //exit
                    user_input.close();
                    View.goodbyeMes();
                    System.exit(0);
                }
            }
            }

        
    }

    //sub controllers

    /** controller to load files to the sgr */
    public static void loadFilesController(GestReviews sgr) {
        Scanner user_input = new Scanner(System.in);
        View.startupApp();
        String response = user_input.nextLine();

        if(response.equals("yes") || response.equals("y") || response.equals("sim")) {
            View.paths(1);
            String bus = user_input.nextLine();
            View.paths(2);
            String rev = user_input.nextLine();
            View.paths(3);
            String usr = user_input.nextLine();
            try {
                sgr.loadSGRFromFiles(true, bus, rev, usr);
                View.print("Loading Files (.csv): " + Crono.getTime());
            }catch (IOException e) {
                View.noDatabase();
                sgr.setFilepaths(GestReviews.bus_default_path, GestReviews.rev_default_path, GestReviews.usr_default_path);               //Safe guard
                Controller.loadFilesController(sgr);
            }
        }
        else if(response.equals("binary")) {
            try {
                sgr = new GestReviews("db/gestReviews.dat");
                View.print("Loading Files (.csv): " + Crono.getTime());
            } catch (IOException | ClassNotFoundException e) {
                View.noDatabase();
                Controller.loadFilesController(sgr);
            }
        }
        else{
            try {
                sgr.loadSGRFromFiles(false, null, null, null);
                View.print("Loading Files (.csv): " + Crono.getTime());
            } catch (IOException e) {
                View.noDatabase();
                Controller.loadFilesController(sgr);
            }
        }
    }

    /** Gives a better control to the month statistic */
    public static void statisticsController(Stats statistics) {
        Scanner user_input = new Scanner(System.in);
        String response = "";
        while(!response.equals("quit")) {
            View.questions(12);
            response = user_input.nextLine();

            if(response.equals("all")) {
                View.print(statistics.tostring2(true,null,0));
                View.print(Crono.getTime());
            }

            else if(response.equals("one")){
                View.questions(13);
                String month = user_input.nextLine();
                View.questions(2);
                int year = readInt(user_input);
                View.print(statistics.tostring2(false,month,year));
            }
        }
    }

    public static int readInt(Scanner user_input){
        int r;
        try{
            r = user_input.nextInt();
        }
        catch (Exception e){
            View.questions(14);
            user_input.nextLine();
            r = readInt(user_input);
        }
        return r;
    }

}
   

