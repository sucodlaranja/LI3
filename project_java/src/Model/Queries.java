package Model;

import Controller.Crono;
import Controller.GestReviews;

import Model.Businesses.BusinessInfo;
import Model.Businesses.Businesses;
import Model.Reviews.Reviews;
import Model.Reviews.ReviewsInfo;
import Model.Users.UsersInfo;

import java.util.*;
import java.util.function.ToIntFunction;
import java.util.stream.Collectors;

/**
 * Model.Queries
 */
public class Queries {

    // Lista ordenada alfabeticamente de business_ids que não tem rating e o seu total
    public static List<String> query_1(GestReviews sgr){
        Crono.start();
        List<String> q1 = sgr.getBusinesses().getNonEvaluated_businesses_ids();
        q1.sort(Comparator.comparing(String::toString));
        Crono.stop();
        return q1;
    }

    // Vamos validar a data aqui dentro
    // binary search é melhor....apenas se chamarmos esta query mais de 50 mil vezes
    public static ArrayList<Integer> query_2(GestReviews sgr, int month, int year) throws NotEnoughData, InvalidArgument {
        Crono.start();
        ArrayList<Integer>  q2 = new ArrayList<>();
        // Get reviews from sgr.
        Reviews reviews = sgr.getReviews();
        // Get index of first review of the respective month/year.
        int idx = reviews.getFirstReviewIndex(year, month);
        if(idx >= 0){
            // Get month starting on the previous index.
            List<ReviewsInfo> monthlyReviews = reviews.getMonthStartedOn(idx);

            // Number of reviews in the respective month
            q2.add(0,monthlyReviews.size());

            // Number of different users that made a review
            HashSet<String> users = new HashSet<>();
            for(ReviewsInfo revInfo : monthlyReviews)
                users.add(revInfo.getUsr_id());
            // Number of different users that made a review
            q2.add(1,users.size());

            // Remove user id's
            users.clear();
        }

        Crono.stop();
        return q2;
    }

    // Para cada mês: quantas reviews fez, nº de negócios diferentes, nota média
    public static String query_3(GestReviews sgr, String usr_id) throws IdNotFound{
        Crono.start();
        // Get user info
        UsersInfo userInfo = sgr.getUsers().getUserInfo(usr_id);

        String q3;
        if(userInfo == null)
            q3 = "Invalid user id.";
        else
            q3 = userInfo.monthlyInfoToString();

        Crono.stop();
        return q3;
    }

    // Para cada mês: número de reviews, nº de users diferentes, média avaliação
    public static String query_4(GestReviews sgr, String bus_id) throws IdNotFound {
        Crono.start();
        // Get business info
        BusinessInfo busInfo = sgr.getBusinesses().getBusinessInfo(bus_id);

        String q4;
        if(busInfo == null)
            q4 = "Invalid business id.";
        else
            q4 = busInfo.monthlyInfoToString();

        Crono.stop();
        return q4;
    }

    // Lista de nomes de negócios que o utilizador avaliou (e quantos) por ordem decrescente (ou alfabética)
    public static List<String> query_5(GestReviews sgr, String usr_id) throws IdNotFound{
        Crono.start();

        Businesses businessInfo = sgr.getBusinesses();

        //Get HashSet with all the businesses ids.
        Set<String> hs = new HashSet<>();
        sgr.getUsers().getUserInfo(usr_id).copyAllHashTable(hs);
        //Transforms the ids to names and sorts them.
        List<String> q5 = new ArrayList<>();
        for (String busId : hs){
            q5.add(businessInfo.getBusinessInfo(busId).getName());
        }
        q5.sort(Comparator.comparing(String::toString));

        Crono.stop();
        return q5;
    }

    // Lista de X negócios com mais reviews por ano e numero de users diferentes que avaliaram cada negocio
    public static List<Pair<Integer, List<Pair<String, Pair<Integer, Integer>>>>>
    query_6(GestReviews sgr, int x) throws NotEnoughData {
        Crono.start();
        int currentYear = sgr.getReviews().getMinDate().getYear();
        int lastYear = sgr.getReviews().getMaxDate().getYear() + 1;

        /*
        Pair<Integer,Integer> -> Pair with number of reviews and number of reviews by different users.
        Pair<String,Pair<Integer,Integer>> -> Pair with the name of the businesses and the pair above.
        List<Pair<String,Pair<Integer,Integer>>>> -> List with all businesses info.
        Pair<Integer,List<Pair<String,Pair<Integer,Integer>>>>
        -> Pair with the year represented by the integer and the list of the top businesses of the year.
        List<Pair<Integer,List<Pair<String,Pair<Integer,Integer>>>>>
        -> List with all years that reviews are made.
         */
        List<Pair<Integer,List<Pair<String,Pair<Integer,Integer>>>>> q6 = new ArrayList<>();

        // Comparator that orders by number of reviews.
        Comparator<Pair<String, Pair<Integer, Integer>>> cNumberReviews =
                Comparator.comparingInt((Pair<String, Pair<Integer, Integer>> pair)
                        -> pair.getSecond().getFirst());

        for (;currentYear < lastYear;currentYear++){
            //PriorityQueue dos top n Businesses.
            PriorityQueue<Pair<String,Pair<Integer,Integer>>> topYear = new PriorityQueue<>(x,cNumberReviews);
            int leastReviews = Integer.MAX_VALUE;

            for(BusinessInfo businessInfo : sgr.getBusinesses().getEvaluatedBusinesses().values()){
                int numberOfReviews = businessInfo.numberOfReviewsYear(currentYear);

                // For the first x Businesses we add to the PriorityQueue.
                if (topYear.size() < x){
                    topYear.add(new Pair<>(businessInfo.getName(),
                            new Pair<>(numberOfReviews,businessInfo.diffUsersReviewsYear(currentYear))));
                    if (leastReviews > numberOfReviews) leastReviews = numberOfReviews;
                }
                // For the rest of the Businesses we add to the PriorityQueue if they qualify.
                else if (leastReviews < numberOfReviews) {
                    topYear.poll();
                    topYear.add(new Pair<>(businessInfo.getName(),
                            new Pair<>(numberOfReviews,businessInfo.diffUsersReviewsYear(currentYear))));
                    assert topYear.peek() != null : "Empty Queue";
                    leastReviews = topYear.peek().getSecond().getFirst();
                }
            }
            // Adds to the list by making the PriorityQueue a sorted list
            q6.add(new Pair<>(currentYear,
                    topYear.stream().sorted(cNumberReviews.reversed()).collect(Collectors.toList())));
        }

        Crono.stop();
        return q6;
    }

    //Para cada cidade, a lista dos três mais famosos negócios em termos de número de reviews
    public static HashMap<String, List<Pair<Integer, String>>> query_7(GestReviews sgr){
        Crono.start();

        /*
        Pair<Integer,String> -> Pair with number of reviews and Business name.
        List<Pair<Integer,String>> -> List with all businesses info.
        HashMap<String,List<Pair<Integer,String>>> -> HashMap that stores the list of business by city.
         */
        HashMap<String,List<Pair<Integer,String>>> q7 = new HashMap<>();

        // Comparator that orders by number of reviews.
        Comparator<Pair<Integer,String>> c =
                Comparator.comparingInt((ToIntFunction<Pair<Integer, String>>) Pair::getFirst).reversed();

        for(BusinessInfo businessInfo : sgr.getBusinesses().getEvaluatedBusinesses().values()){
            String city = businessInfo.getCity();
            int reviews = businessInfo.getnReviews();

            // If we have the city already the city in the HashMap
            if (q7.containsKey(city)){
                List<Pair<Integer,String>> top3 = q7.get(city);
                // For the first 3 business
                if (top3.size() < 3) {
                    top3.add(new Pair<>(businessInfo.getnReviews(),businessInfo.getName()));
                    top3.sort(c);
                }
                // For the rest of the business
                else if ( reviews > top3.get(2).getFirst()){
                        top3.remove(2);
                        top3.add(new Pair<>(reviews,businessInfo.getName()));
                        top3.sort(c);
                    }
                // Sort the list and replace
                q7.replace(city,top3);
            }
            // If we don´t have the city already the city in the HashMap
            else {
                List<Pair<Integer,String>> top3 = new ArrayList<>();
                top3.add(new Pair<>(reviews,businessInfo.getName()));
                q7.put(city,top3);
            }
        }

        Crono.stop();
        return q7;
    }

    //Determinar os códigos dos X utilizadores que avaliaram mais negócios diferentes,
    // por ordem decrescente do número de negócios.
    public static List<Pair<Integer, String>> query_8(GestReviews sgr, int x){
        Crono.start();

        // Comparator that orders by number of different reviews.
        Comparator<Pair<Integer,String>> c =
                Comparator.comparingInt( Pair::getFirst);

        /*
        Pair<Integer,String> -> Pair with number of different reviews and the User name.
        List<Pair<Integer,String>> -> List with all Users info.
         */
        PriorityQueue<Pair<Integer,String>> q8 = new PriorityQueue<>(x,c);

        for (Map.Entry<String,UsersInfo> entry : sgr.getUsers().getUsersMap().entrySet()){

            // Calculate all the different reviews off this user.
            HashSet<String> diffBusinesses = new HashSet<>();
            entry.getValue().copyAllHashTable(diffBusinesses);
            int nReviews = diffBusinesses.size();

            // For the first x users.
            if (q8.size() < x){ q8.add(new Pair<>(nReviews,entry.getKey())); }
            // For the rest of the users.
            else {
                assert q8.peek() != null : "Empty Queue";
                if ( q8.peek().getFirst() < nReviews ){
                    q8.poll();
                    q8.add(new Pair<>(nReviews,entry.getKey()));
                    }
            }
            }

        Crono.stop();
        return q8.stream().sorted(c.reversed()).collect(Collectors.toList());
    }

    // Dado um negócio, determinar o conjunto dos X users que mais o avaliaram com o valor médio de classificação.
    public static List<Pair<String, Pair<Integer, Float>>> query_9(GestReviews sgr, String bus_id, int x){
        Crono.start();

        /*
        Pair<Integer,Float> -> Pair with number of reviews and the average rating.
        Map<String,Pair<Integer,Float>> -> Map of name of User and their review.
         */
        Map<String,Pair<Integer,Float>> temp = new HashMap<>();

        for (ReviewsInfo reviews : sgr.getReviews().getReviewsList()){
            // Only analyze reviews of the given business.
            if (reviews.getBus_id().equals(bus_id)){
                String user_id = reviews.getUsr_id();

                // If we already have a review by this user.
                if (temp.containsKey(user_id)){
                    Pair<Integer,Float> pair = temp.get(user_id);
                    pair = new Pair<>(pair.getFirst() + 1, pair.getSecond() + reviews.getRating());
                    temp.replace(user_id,pair);
                }
                // If we never had a review by this user.
                else { temp.put(user_id, new Pair<>(1, reviews.getRating())); }
            }
        }

        // Comparator that orders by number of reviews and ,if equal, alphabetic order.
        Comparator<Pair<String,Pair<Integer,Float>>> c = (p1, p2) -> {
            if (p1.getSecond().getFirst().compareTo(p2.getSecond().getFirst()) == 0) {
                return p2.getFirst().compareTo(p1.getFirst()); }
            else { return p1.getSecond().getFirst().compareTo(p2.getSecond().getFirst()); }
        };

        /*
        Pair<Integer,Float> -> Pair with number of reviews and the average rating.
        Pair<String,Pair<Integer,Float> -> Pair of name of User and their review total.
        List<Pair<String,Pair<Integer,Float>>> -> list of top x users.
         */
        PriorityQueue<Pair<String,Pair<Integer,Float>>> q9 = new PriorityQueue<>(x,c);

        HashMap<String, UsersInfo> usersInfoHashMap = sgr.getUsers().getUsersMap();
        for (Map.Entry<String,Pair<Integer,Float>> entry : temp.entrySet()){
            int nReviews = entry.getValue().getFirst();

            // For the First x Users
            if (q9.size() < x){ q9.add(new Pair<>(usersInfoHashMap.get(entry.getKey()).getName()
                    ,new Pair<>(nReviews, entry.getValue().getSecond()/nReviews))); }
            //For the rest of the users
            else {
                assert q9.peek() != null : "Empty Queue";
                if (q9.peek().getSecond().getFirst() < nReviews){
                        q9.poll();
                        q9.add(new Pair<>(usersInfoHashMap.get(entry.getKey()).getName(),new Pair<>(nReviews,
                                entry.getValue().getSecond()/nReviews)));
                    }
            }
        }

        Crono.stop();
        return q9.stream().sorted(c.reversed()).collect(Collectors.toList());
    }

    // Para cada estado, cidade a cidade, a média de classificação de cada negócio.
    public static Map<String, Map<String, List<Pair<String, Float>>>> query_10(GestReviews sgr){
        Crono.start();

        /*
        Pair<String,Float> -> Pair with name of business and the average rating.
        List<Pair<String,Float>> -> List with every business info.
        Map<String,List<Pair<String,Float>>> -> Map with every business in a city.
        Map<String,Map<String,List<Pair<String,Float>>>> -> Map with every business in a state separated by city.
         */
        Map<String,Map<String,List<Pair<String,Float>>>> q10 = new HashMap<>();

        for (BusinessInfo businessInfo : sgr.getBusinesses().getEvaluatedBusinesses().values()){
            String state = businessInfo.getState();
            String city = businessInfo.getCity();

            // if we already have the state in the Map.
            if (q10.containsKey(state)){
                // if we already have the city in the Map.
                if (q10.get(state).containsKey(city)){ q10.get(state).get(city)
                        .add(new Pair<>(businessInfo.getName(),businessInfo.getAverage())); }
                // if not we create every struct needed and insert on the Map.
                else {
                    List<Pair<String,Float>> businessesCity = new ArrayList<>();
                    businessesCity.add(new Pair<>(businessInfo.getName(),businessInfo.getAverage()));
                    q10.get(state).put(city,businessesCity);
                }
            }
            // if not we create every struct needed and insert on the Map.
            else {
                List<Pair<String,Float>> businessesCity = new ArrayList<>();
                businessesCity.add(new Pair<>(businessInfo.getName(),businessInfo.getAverage()));
                Map<String,List<Pair<String,Float>>> businessesByState = new HashMap<>();
                businessesByState.put(businessInfo.getCity(),businessesCity);
                q10.put(state,businessesByState);
            }
        }

        Crono.stop();
        return q10;
    }

}
