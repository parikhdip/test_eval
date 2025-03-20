#include <iostream>
#include <vector>
#include <random> 
#include <fstream>
#include <optional>

using namespace std;

const int maxNumOfPins = 10;
const int maxRounds = 10;

using frame = vector<int>;
using frames = vector<vector<int>>;

class BowlingGame {

    private:
        /* This will store the totalScore. It will get updated at each round*/
        int m_totalScore = 0;

        /* This will keep track of the round number. */
        int m_currentRound = 0;

        /* This will help to add the bonus. */
        int m_bonusCount = 0;
        
        /* This will keep track of number of pins remaining in each round. */
        int m_currPinCount = 0;


        /* This function will generate the ramdom number between 1 to 10. */
        int throwTheBall(int remainingPins) {

            std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
            unsigned int seed;
            urandom.read(reinterpret_cast<char*>(&seed), sizeof(seed));

            std::srand(seed);
            int  num = (std::rand() % remainingPins) + 1;
            cout << "Yay!!! Knocked down " << num << " balls!!!" << endl;
            return num;
        }

        /* This function will calculate score through out the game. */
        void calculateScore(int knockedOutPins, int throwCount) {

            if(m_currentRound < maxRounds && throwCount > 2) {
                throw std::runtime_error("Only up to 2 throws allowed at this round.");
            } else if(m_currentRound == maxRounds && throwCount > 3) {
                throw std::runtime_error("Only up to 3 throws allowed at this round.");
            }
            m_totalScore += knockedOutPins;

            if(m_bonusCount) {
                cout << "Added bonus to the previous round score.." << endl;
                m_totalScore += knockedOutPins;
                cout << "new score is : " << m_totalScore << endl;
                m_bonusCount--;
            }
        }

    public:
        BowlingGame() = default;

        ~BowlingGame() = default;

        /* This is the function to play the game. */
        void playGame(std::optional<vector<vector<int>>>& testVec) {

            cout << "Playing bowling game...." << endl;

            bool isTesting = testVec.has_value();
              
            for(int i=0; i<maxRounds ; i++) {

                m_currentRound++;
                int currPins = maxNumOfPins;
                int numOfChances = 2;
                for(int j=0; j<numOfChances; j++) {
                    int knockedOutPins = isTesting ?
                            testVec.value()[i][j] : throwTheBall(currPins);
                    
                    calculateScore(knockedOutPins, j);
                    currPins -= knockedOutPins;
                    
                    if (currPins == 0) {
                        if (m_currentRound == maxRounds) {
                            numOfChances = numOfChances < 3 ? 
                                    ++numOfChances : numOfChances;
                            currPins = maxNumOfPins;
                        } else {
                            m_bonusCount = (j==0) ? m_bonusCount + 2 : ++ m_bonusCount;
                            break;
                        }
                    }
                }
                cout << "Score after round " << m_currentRound << " is " << m_totalScore << endl;
            }
        cout << "Total score is " << m_totalScore << endl;
    }

    void resetGame() {
        m_currentRound = 0;
        m_bonusCount = 0;
        m_currPinCount = 0;
        m_totalScore = 0;
    }

};

int main() {

    cout << "Welcome to the bowling game...." << endl;
    cout << "You will get " << maxRounds << " rounds in the game !!!" << endl;
    cout << "In each round, you will get two chances to knock down all the pins!!" << endl;
    int input = 0;
    cout << "Please choose option,\n";
    cout << "1 for Test the game...!!\n";
    cout << "2 for Play the game...!!\n";

    cin >> input;

    if(input != 1 && input != 2) {
        cout << "Unknown input.." << endl;
        return -1;
    }
    std::optional<vector<vector<int>>> testVec;
   
    if(input == 1) {
        testVec = vector<vector<int>> {{1, 4}, {4, 5}, {6, 4}, {5, 5},
                {10,0}, {0, 1}, {7, 3}, {6, 4},
                {10, 0}, {2, 8, 6}};
    }

    BowlingGame game;
    game.playGame(testVec);
    
    return 0;
}