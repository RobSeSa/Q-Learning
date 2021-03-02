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
    private static int get_next_action(int[][][] q_table, double epsilon, int row, int col) {
        Random rand = new Random();
        double eps_rand = rand.nextDouble();
        //System.out.println("eps_rand = " + eps_rand);
        int action_num, index;
        int max_index = -1;
        int max = -1;
        // take the greedy action
        if(eps_rand < epsilon) {
            // find the max value
            for(int i = 0; i < Parameters.num_actions; i++) {
                if(q_table[row][col][i] > max) {
                    max = q_table[row][col][i];
                    max_index = i;
                }
            }
            action_num = max_index;
        }
        // take a random action
        else {
            action_num = rand.nextInt(4);
        }
        if(action_num == -1) {
            // still unset so error
            System.out.println("Error getting next action");
            return -1;
        }
        return action_num;
    }

    // get the next location if its valid
    private static State get_next_location(int old_row, int old_col, int action_num) {
        int new_row = old_row, new_col = old_col;
        String action_name = Parameters.actions[action_num];
        if(action_name.compareTo("up") == 0 && old_row > 0) {
            new_row = old_row - 1;
        }
        else if(action_name.compareTo("right") == 0 && old_col < Parameters.MAX_COLS - 1) {
            new_col = old_col + 1;
        }
        else if(action_name.compareTo("down") == 0 && old_row < Parameters.MAX_ROWS - 1) {
            new_row = old_row + 1;
        }
        else if(action_name.compareTo("left") == 0 && old_col > 0) {
            new_col = old_col - 1;
        }
        State new_state = new State(new_row, new_col);
        return new_state;
    }

    // get bext path following Q-table
    private static List<State> get_best_path(int[][][] q_table, int[][] values, int start_row, int start_col) {
        List<State> path = new ArrayList<State>();
        int curr_row = start_row, curr_col = start_col;
        int action_num;
        State temp;
        // keep appending until terminal
        while(!is_terminal(values, curr_row, curr_col)) {
            temp = new State(curr_row, curr_col);
            path.add(temp);
            action_num = get_next_action(q_table, 1, curr_row, curr_col);
            temp = get_next_location(curr_row, curr_col, action_num);
            curr_row = temp.getRow();
            curr_col = temp.getCol();
        }
        temp = new State(curr_row, curr_col);
        path.add(temp);
        return path;
    }
    
    private static void printPath(int[][] values, List<State> path) {
        for(int i=0; i<path.size() - 1; i++) {
            System.out.print("(" + path.get(i).getRow() + ", " + path.get(i).getCol() + ") ");
        }
        System.out.println("(" + path.get(path.size()-1).getRow() + ", " + path.get(path.size()-1).getCol() + ") ");
    }
    
    private static int get_path_cost(int[][] values, List<State> path) {
        int cost = 0;
        State curr;
        for(int i = 0; i < path.size(); i++) {
            curr = path.get(i);
            cost += values[curr.getRow()][curr.getCol()];
        }
        return cost;
    }

    // the actual updating of the q_table
    private static void q_training(int[][][] q_table, int[][] values) {
        State curr_state, old_state;
        int action_num, reward, old_q_value, temp_diff, new_q_value;
        int index, max; 
        for(int episode = 0; episode < 1000; episode++) {
            curr_state = get_rand_start(values);
            // take actions from this random start state until we terminate 
            while(!is_terminal(values, curr_state.getRow(), curr_state.getCol())) {
                // get an action
                action_num = get_next_action(q_table, Parameters.EPSILON, curr_state.getRow(), curr_state.getCol());
                old_state = curr_state;
                // perform the action
                curr_state = get_next_location(curr_state.getRow(), curr_state.getCol(), action_num);
                // receive the reward
                reward = values[curr_state.getRow()][curr_state.getCol()];
                // calculate temporal difference
                old_q_value = q_table[old_state.getRow()][old_state.getCol()][action_num];
                // find the max value
                max = -100000;
                for(int i = 0; i < Parameters.num_actions; i++) {
                    if(q_table[curr_state.getRow()][curr_state.getCol()][i] > max) {
                        max = q_table[curr_state.getRow()][curr_state.getCol()][i];
                    }
                }
                temp_diff = (int) (reward + (Parameters.DISCOUNT_FACTOR * max) - old_q_value);
                // update Q-value
                new_q_value = old_q_value + (int) (Parameters.LEARNING_RATE * temp_diff);
                q_table[old_state.getRow()][old_state.getCol()][action_num] = new_q_value;
            }
        }
    }

    // helper functions for the lols
    private static void printPass() {
        System.out.println("Test passed!! Great job :)");
    }

    private static void printFail() {
        System.out.println("You notice something failed!! Great job :)");
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


        // test get_next_action; need a qtable to test
        int[][][] q_table = new int[Parameters.MAX_ROWS][Parameters.MAX_COLS][Parameters.num_actions];
        for(int row = 0; row < Parameters.MAX_ROWS; row++) {
            for(int col = 0; col < Parameters.MAX_COLS; col++) {
                for(int action_index = 0; action_index < Parameters.num_actions; action_index++) {
                    q_table[row][col][action_index] = 0;
                }
            }
        }

        // call get_next_action with this qtable
        int row=0, col=0;
        q_table[row][col][2] = 10000;
        //int action_num = get_next_action(q_table, Parameters.EPSILON, row, col);
        int action_num = get_next_action(q_table, 1, row, col);
        System.out.println("Testing get_next_action:");
        System.out.println("action_num from " + row + ", " + col + " = " + action_num + "("  + Parameters.actions[action_num] + ")");
        if(action_num == 2) {
            printPass();
        }
        else {
            printFail();
        }

        // test get_next_location
        /*State next_loc = get_next_location(row, col, action_num);
        System.out.println("Next location from (" + row + ", " + col + ") taking action " + Parameters.actions[action_num] +
                           " is (" + next_loc.getRow() + ", " + next_loc.getCol() + ")");
        if(next_loc.getRow() == 1 && next_loc.getCol() == 0) {
            printPass();
        }
        else {
            printFail();
        }*/

        // test get_best_path
        /*List<State> path;
        System.out.println("Testing get_best_path:");
        start = get_rand_start(values);
        for(int j = 0; j < 10; j++) {
            System.out.println("start = (" + start.getRow() + ", " + start.getCol() + ") gives path:");
            row = start.getRow();
            col = start.getCol();
            path = get_best_path(q_table, values, row, col);
            printPath(values, path);

            start = get_rand_start(values);
        }
        printPass();
        */


        // test actual training function
        System.out.println("Testing q_training:");
        q_training(q_table, values);
        System.out.println("Done training q_table");
        List<State> path = get_best_path(q_table, values, 3, 9);
        printPath(values, path);
        int cost = get_path_cost(values, path);
        System.out.println(cost);
        if(cost == 93) {
            printPass();
        }
        else{
            printFail();
        }
    }  
} 
