package Model.Reviews;

import java.time.LocalDate;
import java.util.HashSet;

public class ReviewsMonthlyStats {

    private float totalRating;
    private int numberOfReviews;
    private int numberOfUsers;
    private final HashSet<String> diffUsers;
    private final LocalDate date;

    public ReviewsMonthlyStats(LocalDate date){
        this.numberOfReviews = 0;
        this.numberOfUsers = -1;
        this.totalRating = 0;
        this.diffUsers = new HashSet<>();
        this.date = date;
    }

    public ReviewsMonthlyStats(ReviewsMonthlyStats reviewsMonthlyStats){
        this.numberOfReviews = reviewsMonthlyStats.numberOfReviews;
        this.numberOfUsers = reviewsMonthlyStats.numberOfUsers;
        this.totalRating = reviewsMonthlyStats.totalRating;
        this.diffUsers = new HashSet<>(reviewsMonthlyStats.diffUsers);
        this.date = reviewsMonthlyStats.date;
    }

    public ReviewsMonthlyStats clone() { return new ReviewsMonthlyStats(this); }

    public void update(float rating, String usrId){
        this.numberOfReviews++;
        this.totalRating += rating;
        this.diffUsers.add(usrId);
    }

    public float getAverage(){
        return (this.totalRating / this.numberOfReviews);
    }
    public String getmonth() {return this.date.getMonth().toString();}
    public int getYear() {return  this.date.getYear();}

    public int getDiffUsers(){
        if((this.numberOfUsers == -1) && (!this.diffUsers.isEmpty())){
            this.numberOfUsers = this.diffUsers.size();
            this.diffUsers.clear();
        }
        return this.numberOfUsers;
    }

    public String toString(){
        return this.date.getMonth()
                + " "
                + this.date.getYear()
                + "\nNúmero de reviews: "
                + this.numberOfReviews
                + "\nMédia de classificações: "
                + this.getAverage()
                + "\nNúmero de utilizadores: "
                + this.getDiffUsers()
                + "\n\n";
    }

}
