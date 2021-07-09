package Model.Users;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import javax.imageio.IIOException;
import java.io.IOException;

public class UsersTest {

    public UsersTest() {}

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
    public void testUsersConstructor(){
        Users users = new Users();
        assertTrue(users != null);
    }

    @Test
    public void testInvalidFilepath() {
        Users users = new Users();
        try {
            users.loadFile("invalidpath");
        }
        catch (IOException e){
            assertTrue(true);
        }

        assertEquals(0, users.number());
    }

    @Test
    public void testUsers(){
        Users users = new Users();
        try {
            users.loadFile("db/users_full.csv");
        }
        catch (IOException e){
            e.printStackTrace();
        }
        assertTrue(users.number() > 0);
        assertEquals("users_full.csv", users.getFilename());
    }
}
