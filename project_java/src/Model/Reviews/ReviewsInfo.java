package Model.Reviews;

import java.io.Serializable;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;

public class ReviewsInfo implements Comparable<ReviewsInfo>, Serializable {

    private final String rev_id;
    private final String usr_id;
    private final String bus_id;
    private LocalDate date;
    private final float rating;

    public ReviewsInfo(String rev_id, String usr_id, String bus_id, String dateStr, float rating) {
        this.rev_id = rev_id;
        this.usr_id = usr_id;
        this.bus_id = bus_id;
        this.rating = rating;

        // date: yyyy-MM-dd
        this.date = LocalDate.parse(dateStr, DateTimeFormatter.ofPattern("yyyy-MM-dd"));
    }

    public ReviewsInfo(ReviewsInfo reviewsInfo) {
        this.rev_id = reviewsInfo.rev_id;
        this.usr_id = reviewsInfo.usr_id;
        this.bus_id = reviewsInfo.bus_id;
        this.rating = reviewsInfo.rating;
        this.date = reviewsInfo.date;
    }

    public ReviewsInfo clone() { return new ReviewsInfo(this); }

    public LocalDate getDate(){
        return this.date;
    }
    public String getUsr_id() {
        return usr_id;
    }
    public String getBus_id() {
        return bus_id;
    }
    public float getRating() {
        return this.rating;
    }

    @Override
    public int compareTo(ReviewsInfo reviewsInfo) {
        return this.date.compareTo(reviewsInfo.getDate());
    }

    // Used only as an auxiliar tool to perform binary search
    public void addDay(){
        this.date = this.date.plusDays(1);
    }

}
