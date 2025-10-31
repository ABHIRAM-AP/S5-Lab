
import java.awt.*;
import javax.swing.*;

public class Grid {

    public static void main(String[] args) {
        // Create a frame (window)
        JFrame frame = new JFrame("GridLayout Example");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(400, 300);

        // Create a GridLayout with 2 rows and 3 columns
        frame.setLayout(new GridLayout(5, 2, 5, 5));

        // Add buttons (or any components)
        for (int i = 1; i <= 6; i++) {
            frame.add(new JButton("Button " + i));
        }

        // Make it visible
        frame.setVisible(true);
    }

    private static void setLayout(GridLayout gridLayout) {
        throw new UnsupportedOperationException("Not supported yet.");
    }
}
