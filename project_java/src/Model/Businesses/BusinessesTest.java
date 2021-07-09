package Model.Businesses;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.IOException;

public class BusinessesTest {

    private Businesses businesses;

    public BusinessesTest() {
        this.businesses = new Businesses();
        try {
            this.businesses.loadFile("db/business_full.csv");
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
    public void testBusinessesConstructor(){
        assertTrue(this.businesses != null);
    }

    @Test
    public void testInvalidFilepath(){
        Businesses businesses2 = new Businesses();
        try {
            businesses2.loadFile("invalidpath");
        }
        catch (IOException e){
            assertTrue(true);
        }
        assertEquals(0, businesses2.number());
    }

    @Test
    public void testBusinesses(){
        assertTrue(this.businesses.number() > 0);
        assertEquals("business_full.csv", this.businesses.getFilename());
    }

    @Test
    public void testBusinessesContains(){
        assertTrue(this.businesses.contains("C3clPF-DlQuw5NcDyQOo2A"));
        assertFalse(this.businesses.contains("pWi_hCHf27JgiL1gtDgsyQ")); // invalid id (wrong line format)
    }

}
