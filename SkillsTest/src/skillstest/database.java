package skillstest;

//Import required packages
import java.sql.*;
import java.io.BufferedReader;
import java.io.IOException; 
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.PrintWriter;
import java.io.OutputStream;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;


// I got help from:
// https://stackoverflow.com/questions/11943479/create-html-table-from-sql-table
// https://www.tutorialspoint.com/jdbc/jdbc-create-tables.htm
// https://stackoverflow.com/questions/19016363/java-creating-tables-in-mysql-database
// https://medium.com/@ssaurel/create-a-simple-http-web-server-in-java-3fc12b29d5fd
// https://developer.mozilla.org/en-US/docs/Learn/HTML/Forms/Sending_forms_through_JavaScript

// I assume it'll act like a server and database creation will only run once. The server
// should be up the rest of the time. If it goes down and has to be reactivated the database
// should still be there and database creation should automatically get skipped.
public class database {
  
  private static String jdbcDriver = "com.mysql.jdbc.Driver";
  private static String dbAddress = "jdbc:mysql://localhost/"; //This may need to be modified
  private static String userPass = "?user=root&password=";
  private static String dbName = "Avatar_List";
  private static String userName = "root";
  private static String password = "";

  private static Statement statement;
  private static Connection con;
  
  private Socket connect;
  
  public static void main(String[] args) throws SQLException, ClassNotFoundException, IOException {
    database db = new database();
    
    ServerSocket socket = new ServerSocket(8000);
    
    
  }
  
  public void DbImplementation() throws SQLException {
     try { // Register driver, check for a database and that we can connect to it.
         Class.forName(jdbcDriver);
         con = DriverManager.getConnection(dbAddress + dbName, userName, password);
     } 
     catch (ClassNotFoundException e) {
         e.printStackTrace();
     } 
     catch (SQLException e) { // Database hasn't been created
         createDatabase();
         createTable();
     }
  }

  private void createDatabase() throws SQLException{
    try {
      Class.forName(jdbcDriver);
      con = DriverManager.getConnection(dbAddress + userPass);
      Statement s = con.createStatement();
      int myResult = s.executeUpdate("CREATE DATABASE IF NOT EXISTS " + dbName);
    } 
    catch (ClassNotFoundException | SQLException e) {
      e.printStackTrace();
    }finally {
      if(statement != null) {statement.close();}
      if(con!=null) {con.close();}
    }
  }

  private void createTable() throws SQLException {
    
    // All the users' records will be stored in this table
    String TableName = "CREATE TABLE AvatarList (" 
        + "idNo INT(64) NOT NULL AUTO_INCREMENT,"  
        + "name VARCHAR(100)," 
        + "hobby VARCHAR(100),"  
        + "avatar VARBINARY(1000000), "
        + "PRIMARY KEY(idNo))";  
    
    try {
      Class.forName(jdbcDriver);
      con = DriverManager.getConnection(dbAddress + dbName, userName, password);
      statement = con.createStatement();
      statement.executeUpdate(TableName);
      System.out.println("Table Created");
    }
    catch (SQLException e ) {
        System.out.println("An error has occurred on Table Creation");
        e.printStackTrace();
    }
    catch (ClassNotFoundException e) {
        System.out.println("Mysql drivers were not found");
        e.printStackTrace();
    }finally {
      if(statement != null) {statement.close();}
      if(con!=null) {con.close();}
    }
  }
  
}
