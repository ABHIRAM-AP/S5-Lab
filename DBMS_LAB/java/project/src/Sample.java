
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;

public class Sample extends JFrame {

    // Swing components
    final private JTextField idField, nameField, nationalityField;
    final private JButton insertButton, clearButton;
    final private JLabel statusLabel;

    public Sample() {
        setTitle("Add Author");
        setLocationRelativeTo(null);
        setSize(400, 250);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new GridLayout(5, 2, 10, 10));

        // Labels and text fields
        add(new JLabel("Author ID:"));
        idField = new JTextField();
        add(idField);

        add(new JLabel("Author Name:"));
        nameField = new JTextField();
        add(nameField);

        add(new JLabel("Nationality:"));
        nationalityField = new JTextField();
        add(nationalityField);

        // Insert button
        insertButton = new JButton("Insert Author");
        add(insertButton);

        clearButton = new JButton("Clear Fields");
        add(clearButton);

        // Status label
        statusLabel = new JLabel("");
        add(statusLabel);

        // Button action
        insertButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                insertAuthor();
            }
        });

        clearButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                clearFields();
            }
        });

        setVisible(true);
    }

    private void insertAuthor() {
        try {
            int id = Integer.parseInt(idField.getText());
            String name = nameField.getText();
            String nationality = nationalityField.getText();

            String query = "INSERT INTO author(author_id, name, nationality) VALUES (?, ?, ?)";
            String url = "jdbc:mysql://localhost:3306/bookstore";
            String username = "root";
            String password = "pass123";

            Connection con = DriverManager.getConnection(url, username, password);
            PreparedStatement st = con.prepareStatement(query);

            st.setInt(1, id);
            st.setString(2, name);
            st.setString(3, nationality);

            int rows = st.executeUpdate();
            if (rows > 0) {
                statusLabel.setText("Author inserted successfully!");
            } else {
                statusLabel.setText("Insert failed.");
            }

            st.close();
            con.close();
        } catch (NumberFormatException nfe) {
            statusLabel.setText("Invalid ID!");
        } catch (SQLException se) {
            statusLabel.setText("Database Error: " + se.getMessage());
        }
    }

    private void clearFields() {
        nameField.setText("");
        idField.setText("");
        nationalityField.setText("");
    }

    public static void main(String[] args) {
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            System.err.println("MySQL JDBC Driver not found!");
            e.printStackTrace();
            return;
        }

        new Sample();
    }
}
