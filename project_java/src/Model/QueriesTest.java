package Model;

import Controller.Crono;
import Controller.GestReviews;
import View.*;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class QueriesTest {

    private final GestReviews sgr;

    public QueriesTest(){
        Crono.start();
        this.sgr = new GestReviews();
        try {
            sgr.loadSGRFromFiles(false, null, null, null);
        }catch(IOException e) {
            e.printStackTrace();
        }
        Crono.stop();
        // Show loading time
        View.print("Loading Files: " + Crono.getTime());
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
    public void testQuery1(){
        List<String> q1 = Queries.query_1(sgr);
        QueriesView.show_query_1_5(q1,Crono.getTime(),0,q1.size(),1);
    }

    @Test
    public void testQuery2() throws NotEnoughData, InvalidArgument {
        QueriesView.show_query_2(Queries.query_2(sgr, 1, 2018),Crono.getTime());
    }

    @Test
    public void testQuery3() throws IdNotFound{
        QueriesView.show_query_3(Queries.query_3(sgr, "YoVfDbnISlW0f7abNQACIg"),Crono.getTime());
        // ak0TdVmGKo4pwqdJSTLwWw
        // YoVfDbnISlW0f7abNQACIg
        // _QTxuH-DeQ8FfzLuiKXJsA
        // 7q_hkPQH81z6wAVmRJYG6w
    }

    @Test
    public void testQuery4() throws IdNotFound {
        QueriesView.show_query_4(Queries.query_4(sgr, "6UFFVd-78aXmpZ9Dbe7G_A"),Crono.getTime());
        // 6UFFVd-78aXmpZ9Dbe7G_A
        // NXHW9lTvBYNuf-mWsu8eTg
        // i1GWpxJ1Phm2awhVaTkouA
        // NYTxZDV_G3AdC3INltgTpA
    }

    @Test
    public void testQuery5() throws IdNotFound {
        List<String> q5 = Queries.query_5(sgr, "YoVfDbnISlW0f7abNQACIg");

        QueriesView.show_query_1_5(q5,Crono.getTime(),0,q5.size(),5);
        // ak0TdVmGKo4pwqdJSTLwWw
        // YoVfDbnISlW0f7abNQACIg
        // _QTxuH-DeQ8FfzLuiKXJsA
        // 7q_hkPQH81z6wAVmRJYG6w

    }

    @Test
    public void testQuery6() throws NotEnoughData {
        List<Pair<Integer,List<Pair<String, Pair<Integer,Integer>>>>> q6 = Queries.query_6(sgr, 10);
        QueriesView.show_query_6(q6,Crono.getTime(),0,q6.size());
    }

    @Test
    public void testQuery7(){
        HashMap<String,List<Pair<Integer,String>>> q7 = Queries.query_7(sgr);
        QueriesView.show_query_7(q7,Crono.getTime(),true,""); }

    @Test
    public void testQuery8(){
        List<Pair<Integer,String>> q8 = Queries.query_8(sgr,10);
        QueriesView.show_query_8(q8,Crono.getTime(),0,q8.size());
    }

    @Test
    public void testQuery9(){
        List<Pair<String,Pair<Integer,Float>>> q9 = Queries.query_9(sgr,"NXHW9lTvBYNuf-mWsu8eTg",10);
        QueriesView.show_query_9(q9,Crono.getTime(),0,q9.size());
        // 6UFFVd-78aXmpZ9Dbe7G_A
        // NXHW9lTvBYNuf-mWsu8eTg
        // i1GWpxJ1Phm2awhVaTkouA
        // NYTxZDV_G3AdC3INltgTpA
    }

    @Test
    public void testQuery10(){
        Map<String, Map<String,List<Pair<String,Float>>>> q10 = Queries.query_10(sgr);
        QueriesView.show_query_10(q10,Crono.getTime(),true,"","");
    }
}
