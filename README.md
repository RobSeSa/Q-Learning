# Q-Learning
Testing Q-learning implementations across multiple programming languages

Citations:
Complete Example is from this YouTube video:
https://www.youtube.com/watch?v=iKdlKYG78j4&t=158s

### Plan
- read hyperparameters and matrix from a csv (same for all implementations)
- train the model given this matrix
    - define helper functions for this
        - check terminal
        - get random start state
        - get next action given epsilon randomness and best argmax next state
        - get next location (check out of bounds etc)
        - get the path given by the Q matrix
    - 1000 steps (could make this a hyperparameter)
