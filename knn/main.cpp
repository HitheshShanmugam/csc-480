#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include<algorithm>
#include <cstdio>

// renaming the 2-Dimensional vector structure to matrix.
using matrix = std::vector<std::vector<float>>;

// function which gets documents and their corresponding categories.
void GetExistingUserData(const char* fileName, matrix& existingUserRating, std::vector<std::string>& bookList);

void GetNewUserData(const char* fileName, matrix& newUserRating);

// the user type defined
enum class UserType
{
    EXISTING_USER,
    NEW_USER
};

#define RETURN_IGNORE(x) (void(x))

// structure to store input from user.
struct UserInput
{
    std::vector<float> targetUser;
    UserType targetType;
    uint32_t targetIndex;
    uint32_t bookIndex;
    uint32_t k;
};

// renaming a pair of vectors to nearest neighbour
using NearestNeighbour = std::vector<std::pair<uint32_t, float>>;

class RecommenderSystem
{
public:

    //###### Big four operators ######//

    // copy constructor.
    RecommenderSystem(const RecommenderSystem&) = delete;

    // copy assignment operator.
    const RecommenderSystem& operator = (const RecommenderSystem&) = delete;

    // destructor.
    ~RecommenderSystem();


    //###### Public functions ######//

    // gets training data from input file (Users U1 - U20)
    static void GetTrainingData(const char* inputFile);

    // gets test data from input file (New Users N1-N5)
    static void GetTestData(const char* inputFile);

    // generate recommendation based on user input.
    static void  GenerateRecommendations(const UserInput& newInput);

    // read users input for book recommendation.
    static bool ReadUserInput(UserInput& newInput);

    // prints all book list.
    static void PrintbookList();

    // predict existing rating of new users and calculate mean absolute error for each k-value.
    static void PredictExistingRating(std::vector<std::pair<uint32_t, float>>& meanAbsoluteErrorTable);

    // calculate best k-value from mean absolute error table.
    static uint32_t ComputeBest_K_ValueFromErrorTable(std::vector<std::pair<uint32_t, float>>& meanAbsoluteErrorTable);

    // predict rating of new users for books not rated with the best k-value.
    static void PredictNewRating(const uint32_t k);


private:

    //###### Private functions ######//

    // get singleton instance.
    static RecommenderSystem* getPrivateInstance();

    // private default constructor.
    RecommenderSystem();

    // print all users with ratings.
    void PrintUsers(matrix& pUserList);

    // print book rating.
    static void PrintUsersbookRating(const UserInput& newInput, const float& predictedRating);

    // print mean absolute error table.
    void PrintErrorTable(std::vector<std::pair<uint32_t, float>>& meanAbsoluteErrorTable);

    // get the nearest k-neighbours of target user.
    static void GetNearestNeighbours(const UserInput& newInput, NearestNeighbour& nearestNeighbour);

    // gets the pearson coefficient according to target user and existing user vectors.
    const float GetPearsonCoefficient(const std::vector<float>& targetUser, const std::vector<float>& existingUser) const;

    // predict book ratings for new users according to user input and nearest neighbours { returns predicted value }.
    static const float PredictRating(const UserInput& newInput, const NearestNeighbour& nearestNeighbour);

    // comparator for vector to rank based on decreasing order.
    struct Comparator
    {
        // functional operator which compares two pair structure.
        bool operator () (const std::pair<uint32_t, float>& a, const std::pair<uint32_t, float>& b) const;
    };

    // comparator for vector to rank based on increasing order.
    struct MeanComparator
    {
        // functional operator which compares two pair structure.
        bool operator () (const std::pair<uint32_t, float>& a, const std::pair<uint32_t, float>& b) const;
    };

    // comparator for vector to rank based on decreasing order of predicted rating.
    struct RecommendationComparator
    {
        // functional operator which compares two pair structure.
        bool operator () (const std::pair<UserInput, float>& a, const std::pair<UserInput, float>& b) const;
    };



    //######  Data ######//

    // create existing users rating.
    matrix existingUsersRating;

    // create new users rating.
    matrix newUsersRating;

    // book names list.
    std::vector<std::string> bookList;


    float sqrtf(float val) const;
};
void GetExistingUserData(const char* fileName, matrix& existingUserRating, std::vector<std::string>& bookList)
{
    //open file to read.
    std::ifstream myFile(fileName);

    // string to store from file.
    char* newString = new char[1000];

    //  { book names }
    myFile.getline(newString, 1000);

    // store in process string.
    std::string processString(newString);

    // find ',' character. { csv file format }
    size_t pos = processString.find(',');

    // truncate the string.
    processString = processString.substr(pos + 1);

    // find ',' character.
    pos = processString.find(',');

    // iterate until process string is empty.
    while (pos != -1)
    {
        // store value temporary string.
        bookList.push_back(processString.substr(0, pos));

        // truncate the string.
        processString = processString.substr(pos + 1);

        // find next ',' character.
        pos = processString.find(',');

        if (pos == -1)
        {
            // store value temporary string.
            bookList.push_back(processString);
        }
    }

    // temporary structure.
    std::vector<float> temporaryFloat = {};

    // unwanted value
    *newString = 'x';

    // read until existing user are done.
    while (*newString != ',')
    {
        // clear temporary.
        temporaryFloat.clear();

        // read line { user ratings }
        myFile.getline(newString, 1000);

        if (*newString != ',')
        {
            // store in process string.
            processString = newString;

            // find ',' character.
            pos = processString.find(',');

            // truncate the string. { skipping first column users name }
            processString = processString.substr(pos + 1);

            // find ',' character.
            pos = processString.find(',');

            // iterate until process string is empty.
            while (pos != -1)
            {
                // store value temporary float.
                temporaryFloat.push_back(std::stof(processString.substr(0, pos)));

                // truncate the string.
                processString = processString.substr(pos + 1);

                // find next '\t' character.
                pos = processString.find(',');

                if (pos == -1)
                {
                    // store value temporary int.
                    temporaryFloat.push_back(std::stof(processString));
                }
            }

            // store temporary vector to new matrix vector.
            existingUserRating.push_back(temporaryFloat);
        }
    }

    //delete storage.
    delete[] newString;

    // close file.
    myFile.close();
}

void GetNewUserData(const char* fileName, matrix& newUserRating)
{
    //open file to read.
    std::ifstream myFile(fileName);

    // string to store from file.
    char* newString = new char[1000];

    // skip first line { book names }
    myFile.getline(newString, 1000);

    // store in process string.
    std::string processString = {};

    // find ',' character. { csv file format }
    size_t pos = 0;

    // unwanted value
    *newString = 'x';

    while (*newString != ',')
    {
        // read line { skip existing user ratings }
        myFile.getline(newString, 1000);
    }

    // temporary structure.
    std::vector<float> temporaryFloat = {};

    // read until file is empty.
    while (!myFile.eof())
    {
        // clear temporary.
        temporaryFloat.clear();

        // read line { new user ratings }
        myFile.getline(newString, 1000);

        if (*newString != '\0')
        {
            // store in process string.
            processString = newString;

            // find ',' character.
            pos = processString.find(',');

            // truncate the string. { skipping first column users name }
            processString = processString.substr(pos + 1);

            // find ',' character.
            pos = processString.find(',');

            // iterate until process string is empty.
            while (pos != -1)
            {
                // store value temporary float.
                temporaryFloat.push_back(std::stof(processString.substr(0, pos)));

                // truncate the string.
                processString = processString.substr(pos + 1);

                // find next ',' character.
                pos = processString.find(',');

                if (pos == -1)
                {
                    // store value temporary float.
                    temporaryFloat.push_back(std::stof(processString));
                }
            }

            // store temporary vector to new matrix vector.
            newUserRating.push_back(temporaryFloat);
        }
    }

    //delete storage.
    delete[] newString;

    // close file.
    myFile.close();
}
RecommenderSystem* RecommenderSystem::getPrivateInstance()
{
    static RecommenderSystem instance;
    return &instance;
}

RecommenderSystem::RecommenderSystem()
        :
        existingUsersRating{},
        newUsersRating{},
        bookList{}
{
}

RecommenderSystem::~RecommenderSystem()
{
}

void RecommenderSystem::GetTrainingData(const char* inputFile)
{
    // get class instance.
    RecommenderSystem* pRecommender = RecommenderSystem::getPrivateInstance();

    // read the existing users rating from input file. { row = users, column = books }
    GetExistingUserData(inputFile, pRecommender->existingUsersRating, pRecommender->bookList);
}

void RecommenderSystem::GetTestData(const char* inputFile)
{
    // get class instance.
    RecommenderSystem* pRecommender = RecommenderSystem::getPrivateInstance();

    // read the new users rating from input file. { row = users, column = books }
    GetNewUserData(inputFile, pRecommender->newUsersRating);
}

// get the nearest k-neighbours of target user.
void RecommenderSystem::GetNearestNeighbours(const UserInput& newInput, NearestNeighbour& nearestNeighbour)
{
    // get class instance.
    RecommenderSystem* pRecommender = RecommenderSystem::getPrivateInstance();

    // for each existing user.
    for (uint32_t i = 0; i < pRecommender->existingUsersRating.size(); i++)
    {
        // if target is an existing user and if current iteration user is equal to our target user
        if ((newInput.targetType == UserType::EXISTING_USER) && (newInput.targetIndex == i))
        {
            // skip the same existing user.
            continue;
        }

        // calculate pearson coefficient of target user with all existing users.
        nearestNeighbour.push_back({ i, pRecommender->GetPearsonCoefficient(newInput.targetUser, pRecommender->existingUsersRating[i]) });
    }

    // sort all the vector pairs based on pearson coefficient { ie, similarity, similiar items are placed first }
    std::sort(nearestNeighbour.begin(), nearestNeighbour.end(), Comparator());
}

// predict book ratings for new users according to user input and nearest neighbours { returns predicted value }.
const float RecommenderSystem::PredictRating(const UserInput& newInput, const NearestNeighbour& nearestNeighbour)
{
    // get class instance.
    RecommenderSystem* pRecommender = RecommenderSystem::getPrivateInstance();

    // sum of (product of (rating of users upto k and similarity of users upto k)).
    float numeratorVal = 0.0f;

    // sum of all similarities of users upto k
    float denominatorVal = 0.0f;

    // variable to store number of users rated out of  k-neighbours.
    uint32_t numberOfUsersRated = newInput.k;

    // using weighted average approach of k-neighbours to predict the rating given by target user to book.
    for (uint32_t i = 0; i < newInput.k; i++)
    {
        // filter out users who haven't rated the book.
        if (pRecommender->existingUsersRating[nearestNeighbour[i].first][newInput.bookIndex] == 0.0f)
        {
            // decrement number of users rated.
            numberOfUsersRated--;

            // dont execute, this user.
            continue;
        }

        // calculate numerator of weighted average function. { sum(r(Ui, It) * sim(NU, Ui)) }
        numeratorVal += (pRecommender->existingUsersRating[nearestNeighbour[i].first][newInput.bookIndex] * nearestNeighbour[i].second);

        // calculate denominator of weighted average function. { sum (sim(NU, Ui)) }
        denominatorVal += nearestNeighbour[i].second;
    }

    float predictedRating = 0.0f;

    // check if there exist some users who rated the book.
    if (numberOfUsersRated > 0)
    {
        // check if denominator is zero.
        if (denominatorVal != 0.0f)
        {
            // predicted rating.
            predictedRating = numeratorVal / denominatorVal;
        }
        else
        {
            // set to zero.
            predictedRating = 0.0f;
        }
    }
    else
    {
        // if no users rated the book. { return the average rating of book across all existing users who rated the book. }
        for (uint32_t i = 0; i < pRecommender->existingUsersRating.size(); i++)
        {
            // check if user rated the book.
            if (pRecommender->existingUsersRating[i][newInput.bookIndex] > 0.0f)
            {
                // if yes add to average value.
                predictedRating += pRecommender->existingUsersRating[i][newInput.bookIndex];

                // increment count of users rated.
                numberOfUsersRated++;
            }
        }

        // check if any user rated.
        if (numberOfUsersRated != 0)
        {
            // take average { predicted rating = predicted rating / total number of user rated }
            predictedRating /= (float)numberOfUsersRated;
        }
        else
        {
            // set to zero, if no user rated.
            predictedRating = 0.0f;
        }
    }

    // if negative predicting occurs.
    if (predictedRating < 0.0f)
    {
        // reset to zero.
        predictedRating = 0.0f;
    }

    // return predicted value.
    return (predictedRating);
}

// generate recommendation based on user input.
void RecommenderSystem::GenerateRecommendations(const UserInput& newInput)
{
    // get class instance.
    RecommenderSystem* pRecommender = RecommenderSystem::getPrivateInstance();

    // variable to read number of recommendation.
    uint32_t numberOfRecommendation = 0;

    // vector of pearson coefficients for all existing users with their index. { pair : neighbour index, pearson coefficient }
    NearestNeighbour nearestNeighbour = {};

    // find nearest neighbour for new / existing user.
    pRecommender->GetNearestNeighbours(newInput, nearestNeighbour);

    while (true)
    {
        std::cout << "\nEnter number of book recommendations needed { 1 - %zu }:\n\n>> ", pRecommender->bookList.size();
        RETURN_IGNORE(scanf_s("%u", &numberOfRecommendation));

        if (numberOfRecommendation < 1 || numberOfRecommendation > pRecommender->bookList.size())
        {
            std::cout <<"\n Incorrect value or book list size exceeded! Try again!";
        }
        else
        {
            break;
        }
    }

    // vector to store predicted ratings with user input. { pair = User input, predicted rating }
    std::vector<std::pair<UserInput, float>> recommendations;

    // temporary input variable.
    UserInput tempInput = newInput;

    std::cout <<"\nTop Recommended books (which are not rated by the specified user):";
    // for each book in the target users vector.
    for (uint32_t i = 0; i < newInput.targetUser.size(); i++)
    {
        // if book is not rated by the user.
        if (newInput.targetUser[i] == 0.0f)
        {
            // set book index.
            tempInput.bookIndex = i;

            // add recommendation with input data and predicted rating.
            recommendations.push_back( { tempInput, pRecommender->PredictRating(tempInput, nearestNeighbour) } );
        }
    }

    // rank each recommendation using predicted rating. { decreasing order of predicted rating }
    std::sort(recommendations.begin(), recommendations.end(), RecommendationComparator());

    // if recommendations are less than given user need.
    if (recommendations.size() < numberOfRecommendation)
    {
        // set input to recommendation size.
        numberOfRecommendation = (uint32_t)recommendations.size();
    }

    // print all recommendations.
    for (uint32_t i = 0; i < numberOfRecommendation; i++)
    {
        // print recommendations.
        pRecommender->PrintUsersbookRating(recommendations[i].first, recommendations[i].second);
    }
}

// read users input for book recommendation.
bool RecommenderSystem::ReadUserInput(UserInput& newInput)
{
    // get class instance.
    RecommenderSystem* pRecommender = RecommenderSystem::getPrivateInstance();

    uint32_t selectUser = 0;
    while (true)
    {
        std::cout <<"\n\nSelect User To Recommend books: { Press '0' to exit! }";
        std::cout <<"\n1) Existing User  2)New User\n\n>> ";

        RETURN_IGNORE(scanf_s("%u", &selectUser));

        if (selectUser < 0 || selectUser > 2)
        {
            std::cout <<"\n Sorry Wrong Target! Try again!";
        }
        else if (selectUser == 0)
        {
            return false;
        }
        else
        {
            break;
        }
    }

    newInput.targetType = (UserType)(selectUser - 1);

    // variable to select target user.
    uint32_t targetUser = 0;

    // if existing user.
    if (newInput.targetType == UserType::EXISTING_USER)
    {
        // print existing users and ratings
        //pRecommender->PrintUsers(pRecommender->existingUsersRating);

        while (true)
        {
            std::cout <<"\nEnter Target User: { 1 - %zu }\n\n>> ", pRecommender->existingUsersRating.size();

            RETURN_IGNORE(scanf_s("%u", &targetUser));

            if (targetUser < 1 || targetUser > pRecommender->existingUsersRating.size())
            {
                std::cout <<"\n Sorry Wrong Target! Try again!";
            }
            else if (targetUser == 0)
            {
                return false;
            }
            else
            {
                newInput.targetIndex = (targetUser - 1);
                newInput.targetUser = pRecommender->existingUsersRating[newInput.targetIndex];
                break;
            }
        }
    }
    else
    {
        // print new users and ratings
        //pRecommender->PrintUsers(pRecommender->newUsersRating);

        while (true)
        {
            std::cout <<"\nEnter Target User: { 1 - %zu }\n\n>> ", pRecommender->newUsersRating.size();

            RETURN_IGNORE(scanf_s("%u", &targetUser));

            if (targetUser < 1 || targetUser > pRecommender->newUsersRating.size())
            {
                std::cout <<"\n Sorry Wrong Target! Try again!";
            }
            else if (targetUser == 0)
            {
                return false;
            }
            else
            {
                newInput.targetIndex = (targetUser - 1);
                newInput.targetUser = pRecommender->newUsersRating[newInput.targetIndex];
                break;
            }
        }
    }

    // iterate for each 'k' value.
    while (true)
    {
        std::cout <<"\nEnter value of 'K' for K-nearest rating: { 1 - %zu }\n\n>> ", pRecommender->existingUsersRating.size();
        RETURN_IGNORE(scanf_s("%u", &newInput.k));

        if (newInput.k < 1 || newInput.k > pRecommender->existingUsersRating.size())
        {
            std::cout <<"\n Sorry Wrong K value! Try again!";
        }
        else if (newInput.k == 0)
        {
            return false;
        }
        else
        {
            // break from input loop.
            break;
        }
    }

    return (true);
}

void RecommenderSystem::PrintUsersbookRating(const UserInput& newInput, const float& predictedRating)
{
    // get class instance.
    RecommenderSystem* pRecommender = RecommenderSystem::getPrivateInstance();

    // predicted rating of new user on the book.
    std::cout <<"\nPredicted rating of User:%d on book:%s = %f", (newInput.targetIndex + 1), pRecommender->bookList[newInput.bookIndex].c_str(), predictedRating;
}

// predict existing rating of new users and calculate mean absolute error for each k-value.
void RecommenderSystem::PredictExistingRating(std::vector<std::pair<uint32_t, float>>& meanAbsoluteErrorTable)
{
    // get class instance.
    RecommenderSystem* pRecommender = RecommenderSystem::getPrivateInstance();

    // new input for each user.
    UserInput newInput = {};

    // vector of pearson coefficients for all existing users with their index. { pair : neighbour index, pearson coefficient }
    NearestNeighbour nearestNeighbour = {};

    // predicted value.
    float predictedValue = 0.0f;

    // average absolute error value for each user.
    float averageAbsoluteError = 0.0f;

    // average error value for each book.
    float absoluteError = 0.0f;

    // count of books rated.
    float countOfbooksRated = 0;

    // loop based on k-value
    for (uint32_t i = 0; i < pRecommender->existingUsersRating.size(); i++)
    {
        // assign k-value.
        newInput.k = (i + 1);

        // reset average absolute error value.
        averageAbsoluteError = 0.0f;

        // loop based on new users.
        for (uint32_t j = 0; j < pRecommender->newUsersRating.size(); j++)
        {
            // clear nearest neighbour for each user.
            nearestNeighbour.clear();

            // set target user
            newInput.targetUser = pRecommender->newUsersRating[j];

            // set user type.
            newInput.targetType = UserType::NEW_USER;

            // set user index.
            newInput.targetIndex = j;

            // find nearest neighbour for new user.
            pRecommender->GetNearestNeighbours(newInput, nearestNeighbour);

            // reset absolute error value.
            absoluteError = 0.0f;

            // reset count.
            countOfbooksRated = 0;

            // loop for all books.
            for (uint32_t z = 0; z < pRecommender->bookList.size(); z++)
            {
                // if book 'z' is already rated by the new user 'j'
                if (pRecommender->newUsersRating[j][z] > 0.0f)
                {
                    // increment count of books rated.
                    countOfbooksRated++;

                    // assign book index
                    newInput.bookIndex = z;

                    // predict value for existing rated book with  k-value.
                    predictedValue = pRecommender->PredictRating(newInput, nearestNeighbour);

                    // error rate { | predicted value - actual value | }
                    absoluteError += abs(predictedValue - pRecommender->newUsersRating[j][z]);
                }
            }

            // average of absolute errors of books rated.
            absoluteError /= countOfbooksRated;

            // sum the error value for current user.
            averageAbsoluteError += absoluteError;
        }

        // average of absolute errors of all users.
        averageAbsoluteError /= pRecommender->newUsersRating.size();

        // push the average error value to the mean absolute error table.
        meanAbsoluteErrorTable.push_back( { newInput.k, averageAbsoluteError } );
    }

    // print mean absolute error table.
    pRecommender->PrintErrorTable(meanAbsoluteErrorTable);
}

// calculate best k-value from mean absolute error table.
uint32_t RecommenderSystem::ComputeBest_K_ValueFromErrorTable(std::vector<std::pair<uint32_t, float>>& meanAbsoluteErrorTable)
{
    // sort the table based on best K-value { ie; decreasing order of error rate (reusing comparator code) }
    std::sort(meanAbsoluteErrorTable.begin(), meanAbsoluteErrorTable.end(), MeanComparator());

    // return k-value of last element
    return (meanAbsoluteErrorTable[0].first);
}

// predict rating of new users for books not rated with the best k-value.
void RecommenderSystem::PredictNewRating(const uint32_t k)
{
    std::cout <<"\n\nPredicted Ratings for books not rated by new users!";

    // get class instance.
    RecommenderSystem* pRecommender = RecommenderSystem::getPrivateInstance();

    // new input for each user.
    UserInput newInput = {};

    // assign k-value
    newInput.k = k;

    // predicted rating.
    float predictedRating = 0.0f;

    // vector of pearson coefficients for all existing users with their index. { pair : neighbour index, pearson coefficient }
    NearestNeighbour nearestNeighbour = {};

    // for all new users.
    for (uint32_t i = 0; i < pRecommender->newUsersRating.size(); i++)
    {
        // clear nearest neighbour for each user.
        nearestNeighbour.clear();

        // set target user
        newInput.targetUser = pRecommender->newUsersRating[i];

        // set target type.
        newInput.targetType = UserType::NEW_USER;

        // set user index.
        newInput.targetIndex = i;

        // find nearest neighbour for new user.
        pRecommender->GetNearestNeighbours(newInput, nearestNeighbour);

        std::cout <<"\n";

        // for all books.
        for (uint32_t j = 0; j < pRecommender->bookList.size(); j++)
        {
            // if book is not rated.
            if (pRecommender->newUsersRating[i][j] == 0.0f)
            {
                // assign book index.
                newInput.bookIndex = j;

                // predict rating.
                predictedRating = pRecommender->PredictRating(newInput, nearestNeighbour);

                // print details.
                pRecommender->PrintUsersbookRating(newInput, predictedRating);
            }
        }
    }
}


void RecommenderSystem::PrintUsers(matrix& pUserList)
{
    // get class instance.
    RecommenderSystem* pRecommender = RecommenderSystem::getPrivateInstance();

    for (uint32_t i = 0; i < pRecommender->bookList.size(); i++)
    {
        std::cout <<"\t\t%s", pRecommender->bookList[i].c_str();
    }

    // print existing users and their ratings.
    for (uint32_t i = 0; i < pUserList.size(); i++)
    {
        std::cout <<"\nUser %d:", i;
        for (uint32_t j = 0; j < pUserList[0].size(); j++)
        {
            std::cout <<"\t%f", pUserList[i][j];
        }

    }
}

void RecommenderSystem::PrintbookList()
{
    std::cout <<"\nbooks Available:";

    // get class instance.
    RecommenderSystem* pRecommender = RecommenderSystem::getPrivateInstance();

    // for each book in list:
    for (uint32_t i = 0; i < pRecommender->bookList.size(); i++)
    {
        // print book names.
        std::cout <<"\n %d) %s", (i + 1), pRecommender->bookList[i].c_str();
    }
}

// print mean absolute error table.
void RecommenderSystem::PrintErrorTable(std::vector<std::pair<uint32_t, float>>& meanAbsoluteErrorTable)
{
    std::cout <<"\n\n Mean Absolute Table for all new users using existing ratings!\n";

    for (uint32_t i = 0; i < meanAbsoluteErrorTable.size(); i++)
    {
        std::cout <<"\n K = %d || Absolute Error Value: %f", meanAbsoluteErrorTable[i].first, meanAbsoluteErrorTable[i].second;
    }
}

// gets the pearson coefficient according to target user and existing user vectors.
const float RecommenderSystem::GetPearsonCoefficient(const std::vector<float>& targetUser, const std::vector<float>& existingUser) const
{
    // number of co-rated items. { n }
    float numberOfCorated = 0.0f;

    // coefficient variables.
    float sumOfTarget = 0.0f, sumOfExisting = 0.0f;
    float sumOfTargetExisting = 0.0f, sumOfTargetSquare = 0.0f, sumOfExistingSquare = 0.0f;

    // loop size of elements.
    for (uint32_t i = 0; i < targetUser.size(); i++)
    {
        // only find the coefficient values for co-rated items.
        if (targetUser[i] > 0.0f && existingUser[i] > 0.0f)
        {
            // increment number of co-rated values.
            numberOfCorated++;

            // sum of target elements.
            sumOfTarget += targetUser[i];

            // sum of existing elements.
            sumOfExisting += existingUser[i];

            // sum of Target * Existing elements.
            sumOfTargetExisting += (targetUser[i] * existingUser[i]);

            // sum of square of target elements.
            sumOfTargetSquare += (targetUser[i] * targetUser[i]);

            // sum of square of existing elements.
            sumOfExistingSquare += (existingUser[i] * existingUser[i]);
        }
    }

    float pearsonCoefficient = 0.0f;

    // if co-rated values is 2 and above.
    if(numberOfCorated > 1)
    {
        // calculate pearson coefficient. { p = (n * sum(xy)) - (sum(x)*sum(y)) }
        pearsonCoefficient = ((numberOfCorated * sumOfTargetExisting) - (sumOfTarget * sumOfExisting));

        // check if numerator is not zero.
        if (pearsonCoefficient != 0.0f)
        {
            // calculate denominator value. { ((n*sum(x2)) - (sum(x2))) * ((n * sum(y2)) - (sum(y2))) }
            float denominatorVal = ((numberOfCorated * sumOfTargetSquare) - (sumOfTarget * sumOfTarget)) *
                                   ((numberOfCorated * sumOfExistingSquare) - (sumOfExisting * sumOfExisting));

            // check for negative values or zero to square root.
            if (denominatorVal > 0.0f)
            {
                // { p = p / sqrt(denominator value) }
                pearsonCoefficient /= sqrtf(denominatorVal);
            }
        }
    }
    else
    {
        // avoid this neighbour { dis-similar neighbour }
        pearsonCoefficient = -1.0f;
    }


    // return coefficient value.
    return (pearsonCoefficient);
}

float RecommenderSystem::sqrtf(float val) const {

   return sqrtf(val);
}

// functional operator which compares two pair structure.
bool RecommenderSystem::Comparator::operator()(const std::pair<uint32_t, float>& a, const std::pair<uint32_t, float>& b) const
{
    // return true if second float value of existing pair is less. { 'second' means pearson coefficient in this case }
    return (a.second > b.second);
}

// functional operator which compares two pair structure.
bool RecommenderSystem::RecommendationComparator::operator()(const std::pair<UserInput, float>& a, const std::pair<UserInput, float>& b) const
{
    // return true if second float value of existing pair is less. { 'second' means predicted rating in this case }
    return (a.second > b.second);
}

// functional operator which compares two pair structure.
bool RecommenderSystem::MeanComparator::operator()(const std::pair<uint32_t, float>& a, const std::pair<uint32_t, float>& b) const
{
    // return true if second float value of existing pair is less. { 'second' means, mean error value in this case }
    return (a.second < b.second);
}

//the main function
int main()
{
    // input file. copy the path of the file
    const char *inputFile;
    inputFile = R"("C:\Users\sures\OneDrive - DePaul University\Desktop\hw4-book-names.csv")";

    // gets training data from input file
    RecommenderSystem::GetTrainingData(inputFile);

    // gets test data from input file
    RecommenderSystem::GetTestData(inputFile);

    // mean absolute error table for each k-value
    std::vector<std::pair<uint32_t, float>> meanAbsoluteErrorTable = {};

    // print all book list.
    RecommenderSystem::PrintbookList();

    // predict existing rating of new users and calculate mean absolute error for each k-value.
    RecommenderSystem::PredictExistingRating(meanAbsoluteErrorTable);

    // calculate best k-value from mean absolute error table.
    const uint32_t k = RecommenderSystem::ComputeBest_K_ValueFromErrorTable(meanAbsoluteErrorTable);

    // predict rating of new users for books not rated with the best k-value.
    RecommenderSystem::PredictNewRating(k);

    // structure to store input from user for book recommendations.
    UserInput newInput = {};

    // read users input for book recommendation.
    while (RecommenderSystem::ReadUserInput(newInput))
    {
        // generate recommendation based on user input.
        RecommenderSystem::GenerateRecommendations(newInput);
    }

    return 0;
}