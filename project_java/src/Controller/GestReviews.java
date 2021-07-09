package Controller;

import Model.Businesses.Businesses;
import Model.Reviews.Reviews;
import Model.Users.Users;
import View.View;

import java.io.*;

/**
 * Controller.Controller.
 * */
public class GestReviews implements Serializable {

    // Default database filepaths
    public static final String bus_default_path = "db/business_full.csv";
    public static final String rev_default_path = "db/reviews_1M.csv";
    public static final String usr_default_path = "db/users_full.csv";

    // Database filepaths
    private String bus_path;
    private String rev_path;
    private String usr_path;

    // Catalogs
    private final Businesses businesses;
    private final Users users;
    private final Reviews reviews;

    //initializes Controller.GestReviews with default paths
    public GestReviews() {
        this.bus_path = bus_default_path;
        this.rev_path = rev_default_path;
        this.usr_path = usr_default_path;
        this.businesses = new Businesses();
        this.users = new Users();
        this.reviews = new Reviews();
    }

    /** loads binary */
    public GestReviews(String filename) throws IOException, ClassNotFoundException {
        Crono.start();
        ObjectInputStream is = new ObjectInputStream(new FileInputStream(filename));
        GestReviews temp = (GestReviews) is.readObject();
        this.bus_path = temp.bus_path;
        this.rev_path = temp.rev_path;
        this.usr_path = temp.usr_path;
        this.businesses = temp.businesses;
        this.users = temp.users;
        this.reviews = temp.reviews;
        is.close();
        Crono.stop();
    }

    /** Saves a GestReviews on a binary file */
    public void saveBinary(String filepath) throws IOException {
        Crono.start();
        ObjectOutputStream os = new ObjectOutputStream(new FileOutputStream(filepath));
        os.writeObject(this);
        os.flush();
        os.close();
        Crono.stop();
    }



    // Set custom filepaths
    public void setFilepaths(String bus, String rev, String usr){
        this.bus_path = bus;
        this.rev_path = rev;
        this.usr_path = usr;
    }

    // Load files method
    public void loadSGRFromFiles(boolean custom, String bus, String rev, String usr) throws IOException{
        // Load files from custom filepath
        if(custom) setFilepaths(bus, rev, usr);

        Crono.start();


        // Read Business file
        this.businesses.loadFile(this.bus_path);

        // Read Model.Users.Users file
        this.users.loadFile(this.usr_path);

        // Read Model.Reviews.Reviews file
        this.reviews.loadFile(this.rev_path, this.businesses, this.users);


        Crono.stop();
    }

    public Businesses getBusinesses() { return businesses.clone();
    }

    public Users getUsers() {
        return users.clone();
    }

    public Reviews getReviews() {
        return reviews.clone();
    }
}
