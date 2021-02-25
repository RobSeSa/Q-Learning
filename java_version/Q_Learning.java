import java.io.BufferedReader;  
import java.io.FileReader;  
import java.io.IOException;  
import java.util.*;

public class Q_Learning
{  
    // Start of Q-Learning implementation
    // helper functions
    private static boolean is_terminal(int[][] values, int row, int col) {
        int nonterminal_value = -1;
        if(values[row][col] == nonterminal_value) {
            return false;
        }
        else {
            return true;
        }
    }

    // gets a random valid start state; max row and cols are stored as globals in Parameters.java
    private static State get_rand_start(int[][] values) {
        // create instance of Random class
        Random rand = new Random();
        int start_row = rand.nextInt(Parameters.MAX_ROWS);
        int start_col = rand.nextInt(Parameters.MAX_COLS);
        while(is_terminal(values, start_row, start_col)) {
            start_row = rand.nextInt(Parameters.MAX_ROWS);
            start_col = rand.nextInt(Parameters.MAX_COLS);
        }
        State start = new State(start_row, start_col);
        return start;
    }

    // get the next action following an epsilon greedy choice
    private static int get_next_action(int[][][] q_table, int row, int col) {
        Random rand = new Random();
        double eps_rand = rand.nextDouble();
        System.out.println("eps_rand = " + eps_rand);
        int action_num, index;
        int max_index = -1;
        int max = -1;
        if(eps_rand < Parameters.EPSILON) {
            // find the max value
            for(int i = 0; i < Parameters.num_actions; i++) {
                if(q_table[row][col][i] > max) {
                    max = q_table[row][col][i];
                    max_index = i;
                }
            }
            action_num = max_index;
        }
        else {
            action_num = rand.nextInt(4);
        }
        if(action_num == -1) {
            // still unset so error
            System.out.error("Error getting next action");
            return -1;
        }
        return action_num;
    }

    public static void main(String[] args)   
    {  
        String line = "";  
        String splitBy = ",";  
        List<Double> data = new ArrayList<Double>();
        int index;
        try   
        {  
            //parsing a CSV file into BufferedReader class constructor  
            BufferedReader br = new BufferedReader(new FileReader("/Users/robertsato/Desktop/Graduate School/Winter 2021/CSE 210A - Programming Languages/Q-Learning/matrix_data.csv"));  
            index = 0;
            while ((line = br.readLine()) != null)   //returns a Boolean value  
            {  
                String[] tokens = line.split(splitBy);    // use comma as separator  
                for(int i = 0; i < tokens.length; i++) {
                    System.out.println("index: "+index+"; tokens["+i+"] = " + tokens[i]);
                    data.add(Double.parseDouble(tokens[i]));
                    index++;
                }
            }  
        }   
        catch (IOException e)   
        {  
            e.printStackTrace();  
        }  

        // read data into hyperparameters and values matrix
        Parameters.EPSILON = data.get(0).doubleValue();
        Parameters.DISCOUNT_FACTOR = data.get(1).doubleValue();
        Parameters.LEARNING_RATE = data.get(2).doubleValue();
        Parameters.MAX_COLS = data.get(3).intValue();
        Parameters.MAX_ROWS = data.get(4).intValue();

        System.out.println("Epsilon: " + Parameters.EPSILON);
        System.out.println("max cols: " + Parameters.MAX_COLS);

        // remove first 5 parameters saved above
        for(int i = 0; i < 5; i++) {
            data.remove(0);
        }
        System.out.println("values data:" + data);

        // populate the values matrix of type int
        int[][] values = new int[Parameters.MAX_ROWS][Parameters.MAX_COLS];
        index = 0;
        for(int row = 0; row < Parameters.MAX_ROWS; row++) {
            for(int col = 0; col < Parameters.MAX_COLS; col++) {
                values[row][col] = data.get(index).intValue();
                index++;
            }
        }

        // testing values array
        System.out.println("Printing values matrix");
        for(int row = 0; row < Parameters.MAX_ROWS; row++) {
            for(int col = 0; col < Parameters.MAX_COLS; col++) {
                System.out.print(values[row][col] + " ");
            }
            System.out.println();
        }

        // testing actions global
        System.out.println(Parameters.actions[0]);

        // testing State.java
        State temp = new State(0, 1);
        System.out.println("temp = (" + temp.getRow() + ", " + temp.getCol() + ")");
        temp.setRow(2);
        System.out.println("temp = (" + temp.getRow() + ", " + temp.getCol() + ")");
    
        // test is_terminal
        System.out.println("Testing is_terminal:");
        if(is_terminal(values, temp.getRow(), temp.getCol())) {
            System.out.println("temp is terminal!");
        }
        else {
            System.out.println("temp is not terminal!");
        }

        // test get_rand_start
        System.out.println("Testing get_rand_start:");
        State start = get_rand_start(values);
        for(int i = 0; i < 10; i++) {
            System.out.println("start = (" + start.getRow() + ", " + start.getCol() + ")");
            start = get_rand_start(values);
        }
    }  
} 
