Option pricing related work is gathered together and packaged into Excel Add-in. This repo will be updated constantly, either to add more asset classes and pricing methods or improve/solve outstanding issues. 

After importing the Excel Add-in, the pricing functions for different kind of options can be used directly after specifying method and related parameters. All of the options are under "OptionPricer" function sub category.

<img width="220.85" alt="Screen Shot 2023-01-29 at 00 40 04" src="https://user-images.githubusercontent.com/51311870/215307366-d4ecf1a6-345b-445e-b63d-0fdde0e2840a.png"> <img width="293.3" alt="Screen Shot 2023-01-28 at 12 00 20" src="https://user-images.githubusercontent.com/51311870/215291202-681561a1-d327-4ace-ad5d-6276febd27ec.png">

To date, the following derivatives and corresponding methods are covered:

**European Option**
- Analytic Solution from BSM Formula

**Asian Option**
- Monte Carlo Simulation
- Monte Carlo Simulation with Antithetic Sampling
- Geometric Average Analytic Pricer
- Binary Tree (upcoming)

**American Option**
- PDE Approach
- Binary Tree (upcoming)

<img width="709" alt="Screen Shot 2023-01-29 at 00 20 01" src="https://user-images.githubusercontent.com/51311870/215306663-aaf5f293-c838-45e7-889b-62fc20948811.png">

The following modules provide mathmatical support:
- Box Muller Norm(0, 1) Random Number: situated in MyRandom, one for single random number generation, one for batch generation (return vector<double>)
- Cumulative Density Distribution of Normal Distribution: situated in MathUtilities, return corresponding CDF value
- Expectation: situated in MathUtilities, calculate expected value of certain vector
- Covariance: situated in MathUtilities, calculate covariance between two vectors
- Black Scholes: situated in MathUtilities, calculate BSM option value

Special thanks for the Excel XLL Template from K. Lewis. This repo is for educational purpose only. 

