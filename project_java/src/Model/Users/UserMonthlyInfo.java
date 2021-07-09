package Model.Users;

import java.io.Serializable;
import java.util.HashSet;
import java.util.Set;

public class UserMonthlyInfo implements Serializable {

    private int numberOfReviews;
    private float totalRating;
    private int numberOfBusinesses;
    private final HashSet<String> diffBusinesses;

    public UserMonthlyInfo(){
        this.numberOfReviews = 0;
        this.totalRating = 0;
        this.numberOfBusinesses = -1;
        this.diffBusinesses = new HashSet<>();
    }

    public UserMonthlyInfo(UserMonthlyInfo userMonthlyInfo){
        this.numberOfReviews = userMonthlyInfo.numberOfReviews;
        this.totalRating = userMonthlyInfo.totalRating;
        this.numberOfBusinesses = userMonthlyInfo.numberOfBusinesses;
        this.diffBusinesses = new HashSet<>(numberOfBusinesses);
    }

    public UserMonthlyInfo clone() { return new UserMonthlyInfo(this); }

    public int getNumberOfReviews() {
        return numberOfReviews;
    }

    public int getNumberOfBusinesses() {
        if(this.numberOfBusinesses == -1){
            this.numberOfBusinesses = this.diffBusinesses.size();
        }
        return this.numberOfBusinesses;
    }

    public float getAverageRating() {
        return this.totalRating / this.numberOfReviews;
    }

    public void addToHashSet(Set<String> hs){
        hs.addAll(this.diffBusinesses);
    }

    public void update(float rating, String busId) {
        this.numberOfReviews++;
        this.totalRating += rating;
        this.diffBusinesses.add(busId);
    }

    public String toString(){
        return "Número de reviews: "
                + getNumberOfReviews()
                + "\nNúmero de negócios diferentes: "
                + getNumberOfBusinesses()
                + "\nAvaliação média: "
                + getAverageRating()
                + "\n\n";
    }
}
