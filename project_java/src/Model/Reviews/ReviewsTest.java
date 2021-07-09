package Model.Reviews;

import static org.junit.jupiter.api.Assertions.*;

import Model.Businesses.Businesses;
import Model.Users.Users;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.List;

public class ReviewsTest {

    private final Reviews reviews;
    private final Businesses businesses;

    public ReviewsTest() {
        this.reviews = new Reviews();
        this.businesses = new Businesses();
        Users users = new Users();

        try {
            this.businesses.loadFile("db/business_full.csv");
            users.loadFile("db/users_full.csv");
            this.reviews.loadFile("db/reviews_1M.csv", this.businesses, users);
        }
        catch (IOException e){
            e.printStackTrace();
        }

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
        assertTrue(this.reviews != null);
    }

    @Test
    public void testReview(){
        assertTrue(this.reviews.number() > 0);
        assertEquals(1000000, this.reviews.getIncorrect_reviews() + this.reviews.number());
    }

    @Test
    public void testEvaluatedBusinesses(){
        assertTrue(this.businesses.getEvaluated_businesses() > 0);
        assertEquals(160582, this.businesses.getEvaluated_businesses() + this.businesses.getNonEvaluated_businesses());
    }

}
