package Model;

import Controller.Crono;
import Controller.GestReviews;
import Model.Businesses.Businesses;
import Model.Reviews.Reviews;
import Model.Reviews.ReviewsMonthlyStats;
import Model.Users.Users;
import java.util.List;

/**
 * Model.Stats
 *
 */
public class Stats {

    private final Businesses businesses;
    private final Users users;
    private final Reviews reviews;

    private final int incorrectReviews;
    private final int numberOfBusinesses;
    private final int evaluatedBusinesses;
    private final int nonEvaluatedBusinesses;
    private final int numberOfUsers;
    private final int activeUsers;
    private final int inactiveUsers;
    private final int noImpactReviews;
    private final float averageReviewRating;

    private final List<ReviewsMonthlyStats> monthlyStats;

    public Stats(GestReviews sgr){
        this.businesses = sgr.getBusinesses();
        this.users = sgr.getUsers();
        this.reviews = sgr.getReviews();
        this.incorrectReviews = this.reviews.getIncorrect_reviews();
        this.numberOfBusinesses = this.businesses.number();
        this.evaluatedBusinesses = this.businesses.getEvaluated_businesses();
        this.nonEvaluatedBusinesses = this.businesses.getNonEvaluated_businesses();
        this.numberOfUsers = this.users.number();
        this.activeUsers = this.reviews.getActiveUsers();
        this.inactiveUsers = this.reviews.getInactiveUsers();
        this.noImpactReviews = this.reviews.getNo_impact_reviews();
        this.averageReviewRating = this.reviews.getAverageRating();
        this.monthlyStats = sgr.getReviews().buildMonthlyStats();
    }

    private String getFilenames(){
        return "Business Filename: " +
                this.businesses.getFilename() +
                "\nUsers Filename: " +
                this.users.getFilename() +
                "\nReviews Filename: " +
                this.reviews.getFilename();
    }

    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append(getFilenames())
                .append("\nIncorrect Reviews: ")
                .append(this.incorrectReviews)
                .append("\nNumber of Businesses: ")
                .append(this.numberOfBusinesses)
                .append("\nEvaluated Businesses: ")
                .append(this.evaluatedBusinesses)
                .append("\nNon Evaluated Businesses: ")
                .append(this.nonEvaluatedBusinesses)
                .append("\nNumber of Users: ")
                .append(this.numberOfUsers)
                .append("\nActive Users: ")
                .append(this.activeUsers)
                .append("\nInactive Users: ")
                .append(this.inactiveUsers)
                .append("\nReviews with no impact: ")
                .append(this.noImpactReviews)
                .append("\nAverage Review Rating: ")
                .append(this.averageReviewRating)
                .append("\n\n");


        return sb.toString();
    }

    /**For month statistics */
    public String tostring2(boolean all,String month,int year) {
        StringBuilder sb = new StringBuilder();

        // Monthly Information: iterate over this.monthlyStats
        if(all) {
            Crono.start();
            for(ReviewsMonthlyStats rms : this.monthlyStats)
                sb.append(rms.toString());
            Crono.stop();
        }
        else {
            for(ReviewsMonthlyStats rms : this.monthlyStats)
                if(rms.getmonth().equals(month) && rms.getYear() == year) {
                    return rms.toString();
                }
            return null;
        }


        return sb.toString();
    }

}
