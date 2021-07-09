package Model;

public class NotEnoughData extends Exception {
    public NotEnoughData(){
        super();
    }

    public NotEnoughData(String s){
        super(s);
    }
}
