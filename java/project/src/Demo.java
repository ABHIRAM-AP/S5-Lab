
import java.sql.*;

public class Demo {

    public static void main(String args[]) {
        String query = "SELECT * FROM author";
        String url = "jdbc:mysql://localhost:3306/bookstore";
        String username = "root";
        String password = "pass123";

        try {
            // Load MySQL JDBC Driver
            Class.forName("com.mysql.cj.jdbc.Driver");
            System.out.println("Driver Loaded Successfully");

            // Establish connection
            Connection con = DriverManager.getConnection(url, username, password);
            System.out.println("Connection Established");

            // Create statement and execute query
            Statement st = con.createStatement();
            ResultSet rs = st.executeQuery(query);

            // Iterate through results
            while (rs.next()) {
                String id = rs.getString("author_id");
                String name = rs.getString("name");
                System.out.println(id + " - " + name);
            }

            // Close resources
            rs.close();
            st.close();
            con.close();

        } catch (ClassNotFoundException e) {
            System.err.println("MySQL JDBC Driver not found!");
            e.printStackTrace();
        } catch (SQLException e) {
            System.err.println("Database Error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
