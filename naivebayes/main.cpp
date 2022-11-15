#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
// class category.
enum class Category
{
    GOD,
    SALE,
    CATEGORY_COUNT
};

// structure which represents document and it's category.
struct DocumentCategory
{
    unsigned int documentIndex;
    Category category;
};

// function which gets documents and their categories.
void GetDocumentCategories(const char* fileName, std::vector<DocumentCategory>& newDocumentCategory);

// function to get terms as strings from file.
void GetTerms(const char* fileName, std::vector<std::string>& newTerms);

// function to get matrix of each terms from file.
void GetMatrix(const char* fileName, std::vector<std::vector<float>>& newMatrix);

#define RETURN_IGNORE(x) (void(x))

class NaiveBayesClassifier
{
public:

    //###### Big four operators ######//

    // copy constructor.
    NaiveBayesClassifier(const NaiveBayesClassifier&) = delete;

    // copy assignment operator.
    const NaiveBayesClassifier& operator = (const NaiveBayesClassifier&) = delete;

    // destructor.
    ~NaiveBayesClassifier();


    //###### Public functions ######//

    // trains model with terms file, class file and train matrix file.
    static void TrainModel(const char* termsFile, const char* trainClasses, const char* trainMatrix);

    // tests model with given test matrix file { returns categorized documents }.
    static void TestModel(const char* testMatrix, std::vector<DocumentCategory>& docByCategory);

    // Evaluate accuracy of model by comparing with actual test class file.
    static void EvaluateModel(std::vector<DocumentCategory>& docByCategory, const char* testClasses);

    // outputs the class probability for a particular term file from trained data.
    static void GetTermClassProbabilities(const char* termsFile);


private:

    //###### Private functions ######//

    // get singleton instance.
    static NaiveBayesClassifier* getPrivateInstance();

    // private default constructor.
    NaiveBayesClassifier();

    // get document categorized.
    void GetDocByCategory(std::vector<DocumentCategory>& newDocumentCategory, Category newCategory, std::vector<DocumentCategory>& docByCategory);

    // count term by category
    const float CountTermByCategory(std::vector<std::vector<float>>& newMatrix, std::vector<DocumentCategory>& docByCategory, const uint32_t term);

    // total count of terms by category
    const float TotalTermByCategory(std::vector<std::vector<float>>& newMatrix, std::vector<DocumentCategory>& docByCategory);

    // calculate probability of each terms in document using conditional probabilities. { sum(log(P(ai | Cj))) where 'sum' ranges from '1' to 'category count' }
    const float CalculateTermCategoryProbability(std::vector<std::vector<float>>& newMatrix, const uint32_t docIndex, Category newCategory);

    // calculate category based on all term category probability { max (log(P(Cj)) + sum(log((P(ai | Cj))) }
    const Category FindMaxProbability(std::vector<float>& documentCategoryProbability);


    //######  Data ######//

    // probability of each category { P(Ci) priors }
    float categoryProbability[(unsigned int)Category::CATEGORY_COUNT];

    // probability of each term in a given category { P(Wj | Ci) conditional probabilities }
    std::vector<float> termCategoryProbability;

    // max probability of each document { max (log(P(Cj)) + sum(log((P(ai | Cj))) }
    std::vector<float> maxProbability;


    static float logf(float d);
};

// function which gets documents and their corresponding categories.
void GetDocumentCategories(const char* fileName, std::vector<DocumentCategory>& newDocumentCategory)
{
    // temporary structure to read from file.
    DocumentCategory tempCategory = {};

    // open file to read.
    std::ifstream myFile(fileName);

    // string to store from file.
    char* newString = new char[100];

    // read until file is empty.
    while (!myFile.eof())
    {
        // read document index from file.
        myFile.getline(newString, 10, '\t');

        if (*newString != '\0')
        {
            // if string is not null
            if (newString != nullptr)
            {
                // store document index in temporary structure.
                sscanf_s(newString, "%u", &tempCategory.documentIndex);
            }

            // read class category from file. { delimiting character '\n' by default }
            myFile.getline(newString, 10);

            // if string is not null
            if (newString != nullptr)
            {
                // temporary variable.
                unsigned int tempVariable = 0;

                // convert string value to unsigned integer.
                sscanf_s(newString, "%u", &tempVariable);

                // store class category in temporary structure.
                tempCategory.category = (Category)tempVariable;
            }

            // store temporary category values to new vector.
            newDocumentCategory.push_back(tempCategory);
        }
    }

    // delete storage.
    delete[] newString;

    // close file.
    myFile.close();
}

// function to get terms as strings from file.
void GetTerms(const char* fileName, std::vector<std::string>& newTerms)
{
    // open file to read.
    std::ifstream myFile(fileName);

    // string to store from file.
    char* newString = new char[100];

    // read until file is empty.
    while (!myFile.eof())
    {
        // read term from file. { delimiting character '\n' by default }
        myFile.getline(newString, 100);

        // if string is not null
        if (newString != nullptr && *newString != '\0')
        {
            // store string to vector.
            newTerms.push_back(newString);
        }
    }

    // delete storage.
    delete[] newString;

    // close file.
    myFile.close();
}

// function to get matrix of each terms from file.
void GetMatrix(const char* fileName, std::vector<std::vector<float>>& newMatrix)
{
    // open file to read.
    std::ifstream myFile(fileName);

    // string to store from file.
    char* newString = new char[10000];

    // temporary vector to store floats.
    std::vector<float> tempFloats = {};

    // read until file is empty.
    while (!myFile.eof())
    {
        // clear temporary vector
        tempFloats.clear();

        // read line from file.
        myFile.getline(newString, 10000, '\n');

        if (*newString != '\0')
        {
            // store in process string.
            std::string processString(newString);

            // find '\t' character.
            size_t pos = processString.find('\t');

            // iterate until process string is empty.
            while (pos != -1)
            {
                // store value temporary float.
                tempFloats.push_back(std::stof(processString.substr(0, pos)));

                // truncate the string.
                processString = processString.substr(pos + 1);

                // find next '\t' character.
                pos = processString.find('\t');

                if (pos == -1)
                {
                    // store value temporary float.
                    tempFloats.push_back(std::stof(processString));
                }
            }

            // store temporary vector to new matrix vector.
            newMatrix.push_back(tempFloats);
        }
    }

    // delete storage.
    delete[] newString;

    // close file.
    myFile.close();
}

NaiveBayesClassifier* NaiveBayesClassifier::getPrivateInstance()
{
    static NaiveBayesClassifier instance;
    return &instance;
}

NaiveBayesClassifier::NaiveBayesClassifier()
        :
        categoryProbability{},
        termCategoryProbability{},
        maxProbability{}
{
}

NaiveBayesClassifier::~NaiveBayesClassifier()
{
}

// get document categorized.
void NaiveBayesClassifier::GetDocByCategory(std::vector<DocumentCategory>& newDocumentCategory, Category newCategory, std::vector<DocumentCategory>& docByCategory)
{
    // loop through all document category vector.
    for (uint32_t i = 0; i < newDocumentCategory.size(); i++)
    {
        // if matches given category
        if (newDocumentCategory[i].category == newCategory)
        {
            // add to new vector.
            docByCategory.push_back(newDocumentCategory[i]);
        }
    }
}

// count term by category
const float NaiveBayesClassifier::CountTermByCategory(std::vector<std::vector<float>>& newMatrix, std::vector<DocumentCategory>& docByCategory, const uint32_t term)
{
    // term count
    float numberOfTermsInCategory = 0;

    // loop through all document category vector.
    for (uint32_t i = 0; i < docByCategory.size(); i++)
    {
        // update count of terms
        numberOfTermsInCategory += newMatrix[term][docByCategory[i].documentIndex];
    }

    // return total count.
    return (numberOfTermsInCategory);
}

// total count of terms by category
const float NaiveBayesClassifier::TotalTermByCategory(std::vector<std::vector<float>>& newMatrix, std::vector<DocumentCategory>& docByCategory)
{
    // term count
    float totalNumberTermsInCategory = 0;

    // loop through all document category vector.
    for (uint32_t i = 0; i < docByCategory.size(); i++)
    {
        // loop through all terms in matrix vector.
        for (uint32_t j = 0; j < newMatrix.size(); j++)
        {
            // update count of terms
            totalNumberTermsInCategory += newMatrix[j][docByCategory[i].documentIndex];
        }
    }

    // return total count.
    return (totalNumberTermsInCategory);
}

// calculate probability of each terms in document using conditional probabilities. { sum(log(P(ai | Cj))) where 'sum' ranges from '1' to 'category count' }
const float NaiveBayesClassifier::CalculateTermCategoryProbability(std::vector<std::vector<float>>& newMatrix, const uint32_t docIndex, Category newCategory)
{
    // get private instance.
    NaiveBayesClassifier* pClassifier = NaiveBayesClassifier::getPrivateInstance();

    // product of all term probability.
    float termProbability = 0.0f;

    // for each term.
    for (uint32_t i = 0; i < newMatrix.size(); i++)
    {
        // get number of times term occurs in document.
        float termOccurence = newMatrix[i][docIndex];

        // assigns term category probability based on category.
        termProbability += (termOccurence * logf(pClassifier->termCategoryProbability[((unsigned int)newCategory * newMatrix.size()) + i]));
    }

    // return calculated probability.
    return (termProbability);
}

// calculate category based on all term category probability { max (log(P(Cj)) + sum(log((P(ai | Cj))) }
const Category NaiveBayesClassifier::FindMaxProbability(std::vector<float>& documentCategoryProbability)
{
    // get private instance.
    NaiveBayesClassifier* pClassifier = NaiveBayesClassifier::getPrivateInstance();

    // maximum of probability
    float newMaxProbability = documentCategoryProbability[0];

    // category of maximum probability.
    Category maxCategory = {};

    // iterate all category probabilities obtained for current document.
    for (uint32_t i = 0; i < documentCategoryProbability.size(); i++)
    {
        // if current category probability is higher than previous.
        if (documentCategoryProbability[i] > newMaxProbability)
        {
            // set new max as current category probability.
            newMaxProbability = documentCategoryProbability[i];

            // set category of new max category probability.
            maxCategory = (Category)i;
        }
    }

    // set max probability
    pClassifier->maxProbability.push_back(newMaxProbability);

    // return max category.
    return (maxCategory);
}

// trains model with terms file, class file and train matrix file.
void NaiveBayesClassifier::TrainModel(const char* termsFile, const char* trainClasses, const char* trainMatrix)
{
    std::cout <<"\n Training Model :";

    // get private instance.
    NaiveBayesClassifier* pClassifier = NaiveBayesClassifier::getPrivateInstance();

    //###### FILE READ #####//

    // terms vector.
    std::vector<std::string> newTerms = {};

    // get terms from file. { size = number of terms }
    GetTerms(termsFile, newTerms);

    // document category vector.
    std::vector<DocumentCategory> newDocumentCategory = {};

    // get document category from file. { size = number of documents }
    GetDocumentCategories(trainClasses, newDocumentCategory);

    // matrix vector. { row = terms, columns = document }
    std::vector<std::vector<float>> newMatrix = {};

    // get matrix from file. { row size = number of terms, column size = number of documents }
    GetMatrix(trainMatrix, newMatrix);


    //###### INITIALIZATION ######//

    // reserve by category count * count of terms { for performance }
    pClassifier->termCategoryProbability.reserve((unsigned int)Category::CATEGORY_COUNT * newTerms.size());

    // vector to store categorized documents.
    std::vector<DocumentCategory> docByCategory = {};


    //###### ITERATION ######//

    // for each category
    for (unsigned int i = 0; i < (unsigned int)Category::CATEGORY_COUNT; i++)
    {
        // clear category vector.
        docByCategory.clear();

        // get documents categorized. { Ti }
        pClassifier->GetDocByCategory(newDocumentCategory, (Category)i, docByCategory);

        // probability of each category. { P(Ci) = | Ti | / | D | }
        pClassifier->categoryProbability[i] = (float)docByCategory.size() / (float)newDocumentCategory.size();

        // total number of terms belonging to a particular category of documents. { ni }
        const float totalNumberTermsInCategory = pClassifier->TotalTermByCategory(newMatrix, docByCategory);

        // for each term { Wj }
        for (uint32_t j = 0; j < newTerms.size(); j++)
        {
            // number of given term belonging to a particular category of documents { nij }
            const float numberOfTermsInCategory = pClassifier->CountTermByCategory(newMatrix, docByCategory, j);

            // get conditional probability of each term. { P(Wj | Ci) = (nij + 1) / (ni + |V|) }
            const float conditionalProbability = (numberOfTermsInCategory + 1.0f) / (totalNumberTermsInCategory + newTerms.size());

            // store it in term category probability vector.
            pClassifier->termCategoryProbability.push_back(conditionalProbability);
        }
    }

}

// tests model with given test matrix file { returns categorized documents }.
void NaiveBayesClassifier::TestModel(const char* testMatrix, std::vector<DocumentCategory>& docByCategory)
{
    std::cout <<"\n Testing Model :";

    // get private instance.
    NaiveBayesClassifier* pClassifier = NaiveBayesClassifier::getPrivateInstance();

    //###### FILE READ ######//

    // vector to store test matrix. { row = terms, columns = document }
    std::vector<std::vector<float>> newMatrix = {};

    // get matrix from file. { row size = number of terms, column size = number of documents }
    GetMatrix(testMatrix, newMatrix);


    //###### INITIALIZATION ######//

    // temporary document category.
    DocumentCategory temporaryCategory = {};

    // reserve max probability.
    pClassifier->maxProbability.reserve(newMatrix[0].size());

    // probability of current document belonging to each category.
    std::vector<float> documentCategoryProbability = {};


    //###### ITERATION ######//

    // for each document.
    for (uint32_t i = 0; i < newMatrix[0].size(); i++)
    {
        // add document to temporary category.
        temporaryCategory.documentIndex = i;

        // clear vector.
        documentCategoryProbability.clear();

        // for each category
        for (unsigned int j = 0; j < (unsigned int)Category::CATEGORY_COUNT; j++)
        {
            // probability of current document belonging to category { log(P(Cj)) + sum(log(P(ai | Cj))) where 'sum' ranges from '1' to 'category count' }
            documentCategoryProbability.push_back(logf(pClassifier->categoryProbability[j]) + pClassifier->CalculateTermCategoryProbability(newMatrix, i, (Category)j));
        }

        // classify document based on maximum probability of each category. { max (log(P(Cj)) + sum(log((P(ai | Cj))) }
        temporaryCategory.category = pClassifier->FindMaxProbability(documentCategoryProbability);

        // store in categorized document vector.
        docByCategory.push_back(temporaryCategory);
    }

}

// Evaluate accuracy of model by comparing with actual test class file.
void NaiveBayesClassifier::EvaluateModel(std::vector<DocumentCategory>& docByCategory, const char* testClasses)
{
    std::cout <<"\n Evaluation Result :";

    // get private instance.
    NaiveBayesClassifier* pClassifier = NaiveBayesClassifier::getPrivateInstance();

    //###### FILE READ ######//

    // vector to store categorized documents.
    std::vector<DocumentCategory> testCategory = {};

    // get document category from file. { size = number of documents }
    GetDocumentCategories(testClasses, testCategory);

    //###### INITIALIZATION ######//

    // confusion matrix.
    float confusionMatrix[2][2] = {};


    //###### ITERATION ######//

    // for each document.
    for (uint32_t i = 0; i < docByCategory.size(); i++)
    {
        std::cout <<"\n Test Item: %u - ", i;
        std::cout <<"Predicted Class: %u ", (unsigned int)docByCategory[i].category;
        std::cout <<"Actual Class: %u ", (unsigned int)testCategory[i].category;
        std::cout <<"Probability: %f", pClassifier->maxProbability[i];
    }

    // for each document.
    for (uint32_t i = 0; i < docByCategory.size(); i++)
    {
        if ((testCategory[i].category == Category::GOD) && (docByCategory[i].category == Category::GOD))
        {
            // True Positive (TP)
            confusionMatrix[0][0]++;
        }
        else if ((testCategory[i].category == Category::GOD) && (docByCategory[i].category == Category::SALE))
        {
            // False Negative (FN)
            confusionMatrix[0][1]++;
        }
        else if ((testCategory[i].category == Category::SALE) && (docByCategory[i].category == Category::GOD))
        {
            // False Positive (FP)
            confusionMatrix[1][0]++;
        }
        else if ((testCategory[i].category == Category::SALE) && (docByCategory[i].category == Category::SALE))
        {
            // True Negative (TN)
            confusionMatrix[1][1]++;
        }
    }

    // total value of confusion matrix.
    float total = (confusionMatrix[0][0] + confusionMatrix[0][1]) + (confusionMatrix[1][0] + confusionMatrix[1][1]);

    // accuracy of model. { (TP + TN) / Total }
    float accuracy = (confusionMatrix[0][0] + confusionMatrix[1][1]) / total;

    // print accuracy.
    std::cout <<"\n\nOverall Accuracy: %f\n", accuracy;

}

// outputs the class probability for a particular term file from trained data.
void NaiveBayesClassifier::GetTermClassProbabilities(const char* termsFile)
{
    std::cout <<"\n\n Term Class Probabilities (Press '0' to quit):";

    // get private instance.
    NaiveBayesClassifier* pClassifier = NaiveBayesClassifier::getPrivateInstance();

    //##### FILE READ ######//

    // terms vector.
    std::vector<std::string> newTerms = {};

    // get terms from file. { size = number of terms }
    GetTerms(termsFile, newTerms);



    // input string variable.
    char inputString[100];

    // iterate until user wants (press 'esc' to stop)
    while (true)
    {
        std::cout <<"\n\n Enter term :";

        // read input. { scanf stops at first whitespace character }
        RETURN_IGNORE(scanf_s("%s", inputString, 100));

        // check if user pressed '0' key
        if (*inputString == '0')
        {
            break;
        }
        else
        {
            uint32_t i = 0;

            // iterate the terms and check if the given term exist.
            for ( ; i < newTerms.size(); i++)
            {
                // if the term exist.
                if (newTerms[i].compare(inputString) == 0)
                {
                    // print category 0 probability.
                    std::cout <<"\n Category: GOD   Class Probability:%f", pClassifier->termCategoryProbability[i];

                    // print category 1 probability.
                    std::cout <<"\n Category: SALE   Class Probability:%f", pClassifier->termCategoryProbability[newTerms.size() + i];

                    // break from term iteration.
                    break;
                }
            }

            // if iteration completed.
            if (i == newTerms.size())
            {
                std::cout <<"\n Term doesn't exist, Try again! or press '0' to quit!";
            }
        }
    }
}

float NaiveBayesClassifier::logf(float d) {
    return logf(d);
}

// program start.
int main()
{
    // Files needed. copy and paste the paths of the file
    const char *termsFile;
    termsFile = R"(C:\Users\sures\OneDrive - DePaul University\Desktop\newsgroups5-terms.txt)";
    const char* trainClasses;
    trainClasses= R"(C:\Users\sures\OneDrive - DePaul University\Desktop\newsgroups5-train.csv)";
    const char *trainMatrix;
    trainMatrix = R"(C:\Users\sures\OneDrive - DePaul University\Desktop\newsgroups5-train-labels.csv)";
    const char *testClasses;
    testClasses = R"(C:\Users\sures\OneDrive - DePaul University\Desktop\newsgroups5-test.csv)";
    const char *testMatrix;
    testMatrix = R"(C:\Users\sures\OneDrive - DePaul University\Desktop\newsgroups5-test-labels.csv)";

    // trains model with terms file, class file and train matrix file.
    NaiveBayesClassifier::TrainModel(termsFile, trainClasses, trainMatrix);

    // vector of categorized documents.
    std::vector<DocumentCategory> categorizedDocuments = {};

    // tests model with given test matrix file { returns categorized documents }.
    NaiveBayesClassifier::TestModel(testMatrix, categorizedDocuments);

    // Evaluate accuracy of model by comparing with actual test class file.
    NaiveBayesClassifier::EvaluateModel(categorizedDocuments, testClasses);

    // outputs the class probability for a particular term file from trained data.
    NaiveBayesClassifier::GetTermClassProbabilities(termsFile);

    return 0;
}

