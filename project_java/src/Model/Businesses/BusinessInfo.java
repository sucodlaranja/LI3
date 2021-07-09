package Model.Businesses;

import Model.Pair;

import java.io.Serial;
import java.io.Serializable;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

public class BusinessInfo implements Comparable<BusinessInfo>, Serializable {
    
    private final String id;
    private final String name;
    private final String city;
    private final String state;
    private float ratingSum;    // Used to get monthly average rating.
    private int nReviews;
    private boolean evaluated;

    // List of Pairs: (LocalDate, BusinessMonthlyInfo)
    // BusinessMonthlyInfo: #reviews, #Users and Average rating per month.
    private final List<Pair<LocalDate, BusinessMonthlyInfo>> monthlyInfo;

    public BusinessInfo(String id, String name, String city, String state, int nReviews, float ratingSum) {
        this.id = id;
        this.name = name;
        this.city = city.toLowerCase();
        this.state = state.toLowerCase();
        this.nReviews = nReviews;
        this.ratingSum = ratingSum;
        this.evaluated = false;
        this.monthlyInfo = new ArrayList<>();
    }

    public BusinessInfo(BusinessInfo businessInfo) {
        this.id = businessInfo.id;
        this.name = businessInfo.name;
        this.city = businessInfo.city;
        this.state = businessInfo.state;
        this.ratingSum = businessInfo.ratingSum;
        this.nReviews = businessInfo.nReviews;
        this.evaluated = businessInfo.evaluated;
        this.monthlyInfo = businessInfo.monthlyInfo.stream()
                .map(pair -> new Pair<>(pair.getFirst(),pair.getSecond()))
                .collect(Collectors.toList());
    }

    public BusinessInfo clone(){ return new BusinessInfo(this); }

    public float getAverage() {
        return ratingSum/nReviews;
    }

    public int getnReviews() {
        return nReviews;
    }

    public String getName() {
        return name;
    }

    public String getCity() {
        return city;
    }

    public String getState() {
        return state;
    }

    public String getId() {
        return id;
    }

    @Override
    public int compareTo(BusinessInfo business) {
        // First compare states, then cities
        int r = this.state.compareTo(business.getState());
        return (r == 0) ? this.city.compareTo(business.getCity()) : r;
    }

    public void updateRating(Float stars){
        this.evaluated = true;
        this.nReviews++;
        this.ratingSum+=stars;
    }

    public void updateMonthlyInfo(LocalDate date, String usr_id, float rating) {
        Pair<LocalDate, BusinessMonthlyInfo> pair;      // Pair of given year/month
        int lastIndex = this.monthlyInfo.size() - 1;    // Last index of list.
        LocalDate lastDate = null;                      // last registered date
        boolean update = false;                         // false => BusinessMonthlyInfo.add()

        // If monthly info is not empty, get last registered date.
        if(lastIndex >= 0)
            lastDate = this.monthlyInfo.get(lastIndex).getFirst();

        // If it's from the same month, update info.
        if((lastDate != null)
                && (date.getYear() == lastDate.getYear())
                && (date.getMonthValue() == lastDate.getMonthValue()))
        {
            pair = this.monthlyInfo.get(lastIndex);
            update = true;
        }
        else
            pair = new Pair<>(date, new BusinessMonthlyInfo());

        // Update userMonthlyInfo
        pair.getSecond().update(rating, usr_id);

        // Before updating, must remove outdated info.
        if(update)
            this.monthlyInfo.remove(lastIndex);

        // Add pair with new monthly info
        this.monthlyInfo.add(pair);

    }

    public int numberOfReviewsYear(int year){
        return monthlyInfo.stream().filter(pair -> pair.getFirst().getYear() == year)
                .mapToInt(pair -> pair.getSecond().getNumberOfReviews()).sum();
    }

    public int diffUsersReviewsYear(int year){
        Set<String> diffUsersYear = new HashSet<>();
        monthlyInfo.stream().filter(pair -> pair.getFirst().getYear() == year)
                .map(Pair::getSecond).forEach(mInfo -> mInfo.addToHashSet(diffUsersYear));
        return diffUsersYear.size();
    }

    public String monthlyInfoToString(){
        StringBuilder sb = new StringBuilder();
        for(Pair<LocalDate, BusinessMonthlyInfo> pair : this.monthlyInfo){
            sb.append(pair.getFirst().getMonth())
                    .append(" ")
                    .append(pair.getFirst().getYear())
                    .append("\n")
                    .append(pair.getSecond().toString());
        }

        return sb.toString();
    }
}
