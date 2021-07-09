package Model.Businesses;

import java.io.Serializable;
import java.util.HashSet;
import java.util.Set;

public class BusinessMonthlyInfo implements Serializable {

    private int numberOfReviews;
    private int numberOfUsers;
    private float totalRating;
    private final HashSet<String> diffUsers;

    public BusinessMonthlyInfo(){
        this.numberOfReviews = 0;
        this.numberOfUsers = -1;
        this.totalRating = 0;
        this.diffUsers = new HashSet<>();
    }

    public BusinessMonthlyInfo(BusinessMonthlyInfo businessMonthlyInfo){
        this.numberOfReviews = businessMonthlyInfo.numberOfReviews;
        this.numberOfUsers = businessMonthlyInfo.numberOfUsers;
        this.totalRating = businessMonthlyInfo.totalRating;
        this.diffUsers = new HashSet<>(businessMonthlyInfo.diffUsers);
    }

    public BusinessMonthlyInfo clone(){ return new BusinessMonthlyInfo(this); }

    public void update(float rating, String usr_id){
        this.numberOfReviews++;
        this.totalRating += rating;
        this.diffUsers.add(usr_id);
    }

    public int getNumberOfReviews() {
        return numberOfReviews;
    }

    public int getNumberOfUsers(){
        if(this.numberOfUsers == -1){ this.numberOfUsers = this.diffUsers.size(); }
        return this.numberOfUsers;
    }

    public float getAverageRating() {
        return this.totalRating / this.numberOfReviews;
    }

    public void addToHashSet(Set<String> hs){
        hs.addAll(this.diffUsers);
    }

    public String toString(){
        return "Número de reviews: "
                + getNumberOfReviews()
                + "\nNúmero de utilizadores diferentes: "
                + getNumberOfUsers()
                + "\nAvaliação média: "
                + getAverageRating()
                + "\n\n";
    }
}
