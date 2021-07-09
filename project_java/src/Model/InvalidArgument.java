package Model;

public class InvalidArgument extends Exception {
    public InvalidArgument(){
        super();
    }

    public InvalidArgument(String s){
        super(s);
    }
}
