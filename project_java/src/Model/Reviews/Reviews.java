package Model.Reviews;

import Model.Businesses.Businesses;
import Model.InvalidArgument;
import Model.NotEnoughData;
import Model.Users.Users;

import java.io.IOException;
import java.io.Serializable;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.time.LocalDate;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Reviews implements Serializable {

    private String filename;
    private int incorrect_reviews;
    private int no_impact_reviews;
    private int activeUsers;        // inactive users = number() - activeUsers
    private float averageRating;

    private final List<ReviewsInfo> reviews;

    public Reviews(){
        this.incorrect_reviews = 0;
        this.no_impact_reviews = 0;
        this.activeUsers = 0;
        this.averageRating = 0;
        this.reviews = new ArrayList<>();
    }

    public Reviews(Reviews reviews){
        this.filename = reviews.filename;
        this.incorrect_reviews = reviews.incorrect_reviews;
        this.no_impact_reviews = reviews.no_impact_reviews;
        this.activeUsers = reviews.activeUsers;
        this.averageRating = reviews.averageRating;
        this.reviews = reviews.reviews.stream().map(ReviewsInfo::clone).collect(Collectors.toList());
    }

    public Reviews clone() { return new Reviews(this); }

    public void loadFile(String filepath, Businesses businesses, Users users) throws IOException {
        Stream<String> lines = Files.lines(Paths.get(filepath));
        lines.skip(1).forEach(l -> processLine(l, businesses, users));

        // Sort reviews list by date
        Collections.sort(this.reviews);

        // Traverse sorted list of reviews - update user monthly info
        for(ReviewsInfo revInfo : this.reviews){
            users.updateUserInfoByMonth(revInfo.getDate(), revInfo.getUsr_id(), revInfo.getBus_id(), revInfo.getRating());
            businesses.updateBusinessInfoByMonth(revInfo.getDate(), revInfo.getBus_id(), revInfo.getUsr_id(), revInfo.getRating());
        }

        // Global average rating
        this.averageRating /= number();

        // Set filename
        String[] path = filepath.split("/");
        this.filename = path[path.length - 1];
    }

    private void processLine(String line, Businesses businesses, Users users){
        // Variables needed
        String revId = null, usrId = null, busId = null, date = null;
        int useful = 0, funny = 0, cool = 0;
        float stars = 0;
        boolean valid = false;

        // tokens[]: rid;uid;bid;stars;useful;funny;cool;date(YYYY-MM-DD HH:MM:SS);text(ignore)
        String[] tokens = line.split(";", 9);
        if(tokens.length == 9) {
            try {
                revId = tokens[0];
                usrId = tokens[1];
                busId = tokens[2];
                stars = Float.parseFloat(tokens[3]);
                useful = Integer.parseInt(tokens[4]);
                funny = Integer.parseInt(tokens[5]);
                cool = Integer.parseInt(tokens[6]);
                date = tokens[7].split(" ")[0]; // yyyy-MM-dd
                valid = (users.contains(usrId) && businesses.contains(busId));
            } catch (Exception e) {
                valid = false;
            }
        }
        if(valid){
            // STATISTICS
            if (useful + funny + cool == 0) {       // Reviews with no impact.
                this.no_impact_reviews++;
            }
            if(!users.isActive(usrId)){             // Active users.
                users.setActive(usrId);
                this.activeUsers++;
            }
            this.averageRating += stars;            // Global average rating.

            // BUSINESSES
            businesses.updateRating(busId, stars);  // Update business rating

            // date is formatted: "yyyy-MM-dd"
            ReviewsInfo revInfo = new ReviewsInfo(revId, usrId, busId, date, stars);

            // Add to review list
            this.reviews.add(revInfo);
        }
        else {
            this.incorrect_reviews++;
        }
    }

    public String getFilename(){
        return this.filename;
    }

    public int getIncorrect_reviews() { return incorrect_reviews; }

    public int getNo_impact_reviews() { return no_impact_reviews; }

    public int number(){
        return this.reviews.size();
    }

    public int getActiveUsers() { return activeUsers; }

    public int getInactiveUsers() { return number() - getActiveUsers(); }

    public float getAverageRating() { return averageRating; }

    public List<ReviewsInfo> getReviewsList(){
        return reviews.stream().map(ReviewsInfo::clone).collect(Collectors.toList()); }

    public LocalDate getMaxDate() throws NotEnoughData {
        if (reviews.size() == 0) throw new NotEnoughData( "There is no Reviews ");
        return this.reviews.get(this.reviews.size() - 1).getDate();
    }

    public LocalDate getMinDate() throws NotEnoughData {
        if (reviews.size() == 0) throw new NotEnoughData( "There is no Reviews ");
        return this.reviews.get(0).getDate();
    }

    private boolean validDate(int year, int month) throws NotEnoughData{
        boolean ret = true;
        // Get limit values
        LocalDate maxDate = getMaxDate();
        LocalDate minDate = getMinDate();
        int maxYear = maxDate.getYear();
        int minYear = minDate.getYear();
        int maxMonth = maxDate.getMonthValue();
        int minMonth = minDate.getMonthValue();

        if((month < 1) || (month > 12)) ret = false;
        else if((year < minYear) || (year > maxYear)) ret = false;
        else if((year == minYear) && (month < minMonth)) ret = false;
        else if((year == maxYear) && (month > maxMonth)) ret = false;

        return ret;
    }

    // Get index of the first review of a given year and month
    public int getFirstReviewIndex(int year, int month) throws NotEnoughData, InvalidArgument {
        // Validate date
        if(!validDate(year, month)) throw new InvalidArgument("Date :" + year + " " + month + " is not valid");

        String monthStr = (month < 10)? "0" + month : "" + month;
        String dateStr = year + "-" + monthStr + "-01";
        ReviewsInfo reviewsInfo = new ReviewsInfo("", "", "", dateStr, 0);
        int i = Collections.binarySearch(this.reviews, reviewsInfo);
        while((i < 0) && (reviewsInfo.getDate().getMonthValue() == month)){
            reviewsInfo.addDay();
            i = Collections.binarySearch(this.reviews, reviewsInfo);
        }

        if(reviewsInfo.getDate().getMonthValue() != month)
            throw new NotEnoughData("There is no such date: " + month + "/" + year);

        // Go back to the first review of the month, if needed
        if(i > 0){
            LocalDate currDate = this.reviews.get(i).getDate();
            LocalDate prevDate = this.reviews.get(i-1).getDate();
            while((currDate.getMonthValue() == prevDate.getMonthValue()) && (i > 0)){
                i--;
                currDate = prevDate;
                prevDate = this.reviews.get(i-1).getDate();
            }
        }

        return i;
    }

    private int getMonthReview(int idx){
        return (idx > -1) && (idx < this.reviews.size())?
                this.reviews.get(idx).getDate().getMonthValue() : -1;
    }

    // Get month started in index
    public List<ReviewsInfo> getMonthStartedOn(int idx) throws IndexOutOfBoundsException{
        // List of reviews made in the month starting at the index idx.
        List<ReviewsInfo> monthlyReviews= new ArrayList<>();

        int month = getMonthReview(idx);
        if(month != -1){
            // Check if the next review is still from the same month and add it.
            do{
                monthlyReviews.add(this.reviews.get(idx).clone());
                idx = idx + 1;
            }while(month == getMonthReview(idx));
        }

        return monthlyReviews;
    }

    // Monthly stats
    public List<ReviewsMonthlyStats> buildMonthlyStats(){
        if(this.reviews.isEmpty()) return null;

        List<ReviewsMonthlyStats> monthlyStats = new ArrayList<>();
        LocalDate prevDate = this.reviews.get(0).getDate();
        ReviewsMonthlyStats rms = new ReviewsMonthlyStats(prevDate);

        for(ReviewsInfo revInfo : this.reviews){
            LocalDate newDate = revInfo.getDate();
            if((prevDate.getYear() != newDate.getYear())
                || (prevDate.getMonthValue() != newDate.getMonthValue()))
            {
                // Colocar variável local na lista
                monthlyStats.add(rms.clone());
                // Criar nova instância e dar update da variável local
                rms = new ReviewsMonthlyStats(newDate);
            }
            // Update local variable
            rms.update(revInfo.getRating(), revInfo.getUsr_id());

            // Update prevDate
            prevDate = newDate;
        }

        return monthlyStats;
    }
}
