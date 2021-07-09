package Model.Users;

import Model.Pair;

import java.io.Serializable;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class UsersInfo implements Serializable {

    private boolean active;     // User is active? (aka made a review)
    private final String name;

    // List of Pairs: (LocalDate, UserMonthlyInfo)
    // UserMonthlyInfo: #reviews, #Businesses and Average rating per month.
    private final List<Pair<LocalDate, UserMonthlyInfo>> monthlyInfo;

    public UsersInfo(String name){
        this.active = false;
        this.name = name;
        this.monthlyInfo = new ArrayList<>();
    }

    public UsersInfo(UsersInfo usersInfo){
        this.active = usersInfo.active;
        this.name = usersInfo.name;
        this.monthlyInfo = new ArrayList<>();
        usersInfo.monthlyInfo.forEach(pair -> this.monthlyInfo.add(new Pair<>(pair.getFirst(),pair.getSecond())));
    }

    public UsersInfo clone(){ return new UsersInfo(this); }

    public String getName() { return name;}


    public void updateMonthlyInfo(LocalDate date, String busId, float rating){
        Pair<LocalDate, UserMonthlyInfo> pair;          // Pair of given year/month
        int lastIndex = this.monthlyInfo.size() - 1;    // Last index of list.
        LocalDate lastDate = null;                      // last registered date
        boolean update = false;                         // false => monthlyInfo.add()

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
            pair = new Pair<>(date, new UserMonthlyInfo());

        // Update userMonthlyInfo
        pair.getSecond().update(rating, busId);

        // Before updating, must remove outdated info.
        if(update)
            this.monthlyInfo.remove(lastIndex);

        // Add pair with new monthly info
        this.monthlyInfo.add(pair);

    }

    public boolean isActive() {
        return active;
    }

    public void setActive() {
        this.active = true;
    }

    public void copyAllHashTable(Set<String> hs){
        monthlyInfo.stream().map(Pair::getSecond).forEach(userMonthlyInfo -> userMonthlyInfo.addToHashSet(hs));
    }

    public String monthlyInfoToString(){
        StringBuilder sb = new StringBuilder();
        for(Pair<LocalDate, UserMonthlyInfo> pair : this.monthlyInfo){
            sb.append(pair.getFirst().getMonth())
                    .append(" ")
                    .append(pair.getFirst().getYear())
                    .append("\n")
                    .append(pair.getSecond().toString());
        }
        return sb.toString();
    }
}
