package Model;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import Controller.GestReviews;
import Model.Businesses.Businesses;
import Model.Reviews.Reviews;
import Model.Users.Users;

import java.io.IOException;

public class StatsTest {

    private final Businesses businesses;
    private final Users users;
    private final Reviews reviews;
    private final Stats statistics;

    public StatsTest(){
        GestReviews sgr = new GestReviews();
        try {
            sgr.loadSGRFromFiles(false, null, null, null);
        }catch (IOException e) {
            e.printStackTrace();
        }
        this.statistics = new Stats(sgr);
        this.businesses = sgr.getBusinesses();
        this.users = sgr.getUsers();
        this.reviews = sgr.getReviews();
    }

    /**
     * Sets up the test fixture.
     *
     * Called before every test case method.
     */
    @BeforeEach
    public void setUp() {}

    /**
     * Tears down the test fixture.
     *
     * Called after every test case method.
     */
    @AfterEach
    public void tearDown() {}

    @Test
    public void testConstructor(){
        assertNotNull(this.businesses);
        assertNotNull(this.users);
        assertNotNull(this.reviews);
    }

    @Test
    public void testStats(){
        System.out.println(this.statistics);
    }
}
