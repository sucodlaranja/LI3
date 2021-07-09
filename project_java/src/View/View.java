package View;

/**
 * View.View.
 * */
public class View {


    /** Startup application */
    public static void startupApp(){
        System.out.println("GestReviewsApp by group 88\n");
        System.out.print("Do you want to specify a custom database filepath? Or do you want to load a binary file? [Y/N/binary]: ");
        
    }

    /** Queries Menu View */
    public static void queriesMenu() {

        String sb = "---------------------\tQueries Menu\t---------------------\n" + "(1) - Lista ordenada alfabeticamente com os identificadores dos negocios.\n" +
                "(2) - Dado um mês e um ano (válidos), determinar o número total global de reviews realizadas e o número total de users distintos que as realizaram.\n" +
                "(3) - Dado um código de utilizador, determinar, para cada mês, quantas reviews fez,quantos negócios distintos avaliou e que nota média atribuiu.\n" +
                "(4) - Dado o código de um negócio, determinar, mês a mês, quantas vezes foi avaliado,por quantos users diferentes e a média de classificação.\n" +
                "(5) - Dado o código de um utilizador determinar a lista de nomes de negócios que mais" +
                "avaliou (e quantos), ordenada por ordem decrescente de quantidade e, para" + "quantidades iguais, por ordem alfabética dos negócios.\n" +
                "(6) - Determinar o conjunto dos X negócios mais avaliados (com mais reviews) em cada" +
                "ano, indicando o número total de distintos utilizadores que o avaliaram.\n" +
                "(7) - Determinar, para cada cidade, a lista dos três mais famosos negócios em termos de número de reviews.\n" +
                "(8) - Determinar os códigos dos X utilizadores (sendo X dado pelo utilizador) que " +
                "avaliaram mais negócios diferentes, indicando quantos, sendo o critério de ordenação a ordem decrescente do número de negócios.\n" +
                "(9) - Dado o código de um negócio, determinar o conjunto dos X users que mais o " +
                "avaliaram e, para cada um, qual o valor médio de classificação.\n" +
                "(10) - Determinar para cada estado, cidade a cidade, a média de classificação de cada negócio.\n" +
                "(11) - Back to Main menu\n";
        System.out.print(sb);
    }

    /** Main Menu View */
    public static void mainMenu() {
        String sb = "---------------------\tMenu\t---------------------\n" +
                    "(1) - Dados referentes aos ultimos ficheiros lidos\n" +
                    "(2) - Numeros gerais por mes\n" +
                    "(3) - Queries\n" +
                    "(4) - Load new sgr\n" +
                    "(5) - Save Binary\n" +
                    "(6) - Load Binary\n" +
                    "(7) - Exit Program\n";
        System.out.print(sb);
    }

    /**Questions about the filepaths */
    public static void paths(int value) {
        if(value == 1) System.out.print("Please insert bus path: ");
        else if (value == 2) System.out.print("Please insert rev path: ");
        else System.out.print("Please insert usr path: ");
    }

    public static void questions(int value) {
        if(value == 1) System.out.print("Please insert a month(int): ");
        else if(value == 2) System.out.print("Please insert a year: ");
        else if(value == 3) System.out.print("Please insert a user id: ");
        else if(value == 4) System.out.print("Please insert a business id: ");
        else if(value == 5) System.out.print("Please insert a number of businesses: ");
        else if(value == 6) System.out.print("Please insert a number of users: ");
        else if(value == 7) System.out.print("Please insert a option: ");
        else if(value == 8) System.out.print("Continue? ");
        else if(value == 9) System.out.print("[next/previous/quit]: ");
        else if(value == 10) System.out.print("Please insert a state: ");
        else if(value == 11) System.out.print("Please insert a city: ");
        else if(value == 12) System.out.print("Do you want to see all information or just one in specific? [one/all/quit]: ");
        else if(value == 13) System.out.print("Please insert a month(String): ");
        else if(value == 14) System.out.print("Please insert an int: ");



    }

    /**Headers for some of the queries */
    public static void headers(int value) {
        if(value == 1) System.out.println("Business_id's");
        else if(value == 2) System.out.println("User_id's");

    }
    public static void print(String s){
        if(s == null) View.printError();
        else System.out.println(s);
    }
    public static void printError() {System.out.println("No data to output");}
    public static void noDatabase() {System.out.println("No Database loaded.");}

    public static void goodbyeMes(){
        System.out.println("Goodbye World!");
    }

}