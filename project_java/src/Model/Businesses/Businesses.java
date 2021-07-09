package Model.Businesses;

import Model.IdNotFound;

import java.io.IOException;
import java.io.Serializable;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.time.LocalDate;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Businesses implements Serializable {

    // Businesses evaluated vs not evaluated
    private final Map<String, BusinessInfo> businessesE;
    private final Map<String, BusinessInfo> businessesNE;

    // non-evaluated = number() - evaluated;
    private int evaluated_businesses;
    private String filename;

    public Businesses(){
        this.businessesE = new HashMap<>();
        this.businessesNE = new HashMap<>();
        this.evaluated_businesses = 0;
    }

    public Businesses(Businesses businesses){
        this.businessesE = new HashMap<>();
        businesses.businessesE.forEach((s, businessInfo) -> this.businessesE.put(s,businessInfo.clone()));
        this.businessesNE = new HashMap<>();
        businesses.businessesNE.forEach((s, businessInfo) -> this.businessesNE.put(s,businessInfo.clone()));
        this.evaluated_businesses = businesses.evaluated_businesses;
        this.filename = businesses.filename;
    }

    public Businesses clone() { return new Businesses(this); }

    public String getFilename() {
        return this.filename;
    }

    public int getEvaluated_businesses() {
        return this.evaluated_businesses;
    }

    public int getNonEvaluated_businesses() {
        return this.businessesNE.size();
    }

    public Map<String, BusinessInfo> getEvaluatedBusinesses() {
        Map<String, BusinessInfo> r = new HashMap<>();
        businessesE.forEach((s, businessInfo) -> r.put(s,businessInfo.clone()));
        return r;
    }

    public List<String> getNonEvaluated_businesses_ids(){
        return this.businessesNE.values().stream().map(BusinessInfo::getId).collect(Collectors.toList());
    }

    public BusinessInfo getBusinessInfo(String busId) throws IdNotFound {
        BusinessInfo bi;
        if ((bi = this.businessesE.get(busId)) == null){
            if ((bi = this.businessesNE.get(busId)) == null) {
                throw new IdNotFound("Business id not found :" + busId);
            }
        }
        return bi.clone();
    }

    public void loadFile(String filepath) throws IOException{
        Stream<String> lines = Files.lines(Paths.get(filepath));
        lines.skip(1).forEach(this::createBusinessFromLine);

        // Set filename
        String[] path = filepath.split("/");
        this.filename = path[path.length - 1];
    }

    public void createBusinessFromLine(String line){
        String[] tokens = line.split(";"); // tokens[]: id - name - city - state - categories (ignore)
        if(tokens.length == 4 || tokens.length == 5){
            this.businessesNE.put(tokens[0], new BusinessInfo(tokens[0], tokens[1], tokens[2], tokens[3], 0, 0));
        }
    }

    // Returns number of businesses processed.
    public int number(){
        return this.businessesE.size() + this.businessesNE.size();
    }

    public boolean contains(String id){
        return this.businessesE.containsKey(id) || this.businessesNE.containsKey(id);
    }

    public void updateRating(String id, Float stars){
        // Get business info & Update Rating
        BusinessInfo busInfo = this.businessesNE.get(id);

        // Case it has already been evaluated.
        if(busInfo == null) {
            busInfo = this.businessesE.get(id);
            busInfo.updateRating(stars);
            this.businessesE.replace(id, busInfo);
        }
        else{
            this.evaluated_businesses++;
            busInfo.updateRating(stars);
            this.businessesE.put(id, busInfo);
            this.businessesNE.remove(id);
        }

    }

    public void updateBusinessInfoByMonth(LocalDate date, String bus_id, String usr_id, float rating) {
        boolean ne = false;
        // Get user info
        BusinessInfo busInfo = this.businessesE.get(bus_id);
        if(busInfo == null){
            ne = true;
            busInfo = this.businessesNE.get(bus_id);
        }

        // Update user info where key = year + month
        busInfo.updateMonthlyInfo(date, usr_id, rating);

        // Update user info in users table
        if(ne)
            this.businessesNE.replace(bus_id, busInfo);
        else
            this.businessesE.replace(bus_id, busInfo);
    }
}
