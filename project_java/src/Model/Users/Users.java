package Model.Users;

import Model.IdNotFound;

import java.io.IOException;
import java.io.Serializable;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.time.LocalDate;
import java.util.HashMap;
import java.util.stream.Stream;

public class Users implements Serializable {

    private HashMap<String, UsersInfo> users;
    private String filename;

    public Users(){
        this.users = new HashMap<>();
    }

    public Users(Users users){
        this.filename = users.filename;
        this.users = new HashMap<>();
        users.users.forEach((k,e) -> this.users.put(k,e.clone()));
    }

    public Users clone() { return new Users(this); }

    public void loadFile(String filepath) throws IOException {
        Stream<String> lines = Files.lines(Paths.get(filepath));
        lines.skip(1).forEach(this::addUsrFromLine);

        // Set filename
        String[] path = filepath.split("/");
        this.filename = path[path.length - 1];
    }

    public void addUsrFromLine(String line){
        String[] tokens = line.split(";");
        if(tokens.length == 3){
            UsersInfo usrInfo = new UsersInfo(tokens[1]);
            this.users.put(tokens[0], usrInfo); // tokens[0] == user_id
        }
    }

    // User & Business are valid.
    public void updateUserInfoByMonth(LocalDate date, String usrId, String busId, float rating){
        // Get user info
        UsersInfo userInfo = this.users.get(usrId);

        // Update user info where key = year + month
        userInfo.updateMonthlyInfo(date, busId, rating);

        // Update user info in users table
        this.users.replace(usrId, userInfo);
    }

    public HashMap<String, UsersInfo> getUsersMap(){
        HashMap<String, UsersInfo> r = new HashMap<>();
        users.forEach((k,e) -> r.put(k,e.clone()));
        return r; }

    // Returns number of users processed.
    public int number(){
        return this.users.size();
    }

    public boolean contains(String id){
        return this.users.containsKey(id);
    }

    public String getFilename() {
        return this.filename;
    }

    public boolean isActive(String id){
        return this.users.get(id).isActive();
    }

    public void setActive(String id){
        this.users.get(id).setActive();
    }

    public UsersInfo getUserInfo(String id) throws IdNotFound {
        UsersInfo usersInfo;
        if ((usersInfo = users.get(id)) != null){ return usersInfo.clone();}
        else throw new IdNotFound("User id not found :" + id);
    }
}

