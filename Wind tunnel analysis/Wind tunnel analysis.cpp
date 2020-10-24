//This program was written to help analysis data from This software. 
//It takes an unmodified output file from this program as an input and outputs more useful data.
//This instance is set up to compute data useful when working with airfoils.
//Last edits done on 9-22-2020

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

class instance //This class is used to store all data for a single instance of collected data from the wind tunnel
{
public:
    //All this data comes from the wind tunnel output file.
    int Date;           //stores the given date of an instance
    string Time;        //stores the time an instance was taken
    double q = 0;           //don't actually know what this is. units of psf.
    double V_ref = 0;       //inputs mph
    double Alpha = 0;       //inputs deg
    double NormalForce = 0; //inputs lbf
    double AxialForce = 0;  //inputs lbf
    double Moment = 0;      //inputs in*lbf
    double P = 0;           //also don't konw what this is.
    string Orientation; //don't know what this is.

    //The following values are computed values.
    double LiftForce;
    double DragForce;
    double LiftCoefficent;
    double DragCoefficent;
    double CorrectedAlpha; //Both of these corrected values are found using fudge factor equations given by the wind tunnel manufactuaer to 
    double CorrectedDragCoefficent;  //account for the fact that the wind tunnel is a constrained enviroment. 

    void GetData(istream& input); //gets the data for a single instance from a file
    void WriteData(ofstream& output, int spacing);//outputs the data for a single instance to a file. Might add more to allow uset to choose what data to output
    void Compute(double Density, double Area); // handles all the data analysis for a single instance.
    void SetZero(); // sets all relavent variabls to zero
};


void InputData(vector<instance>& DataPoints); // deals with reading all the data in from a txt file.
void OutputData(vector<instance>& DataPoints); // Deals with outputing all the data to a txt file.
void AverageData(vector<instance>& DataPoints, vector<instance>& DataPointsAveraged); //Takes several data points from DataPoints that were taken at the same time and averages them into a single value in DataPointsAveraged
void Compute(vector<instance>& AvgData); // Deals with all the computation things.

int main()
{
    
    vector<instance> RawData;
    vector<instance> AvgData;
    InputData(RawData); // Gets the data from a user provided file and stores it in RawData
    AverageData(RawData, AvgData); // Averages the data from RawData and puts it in AvdData
    Compute(AvgData); // Deals with all the mathmatics
    OutputData(AvgData); // Outputs all the data to a userspecified file.
}


void instance::Compute(double Density, double Area)// deals with all the math. 
{
    //Density units lbm/ft^3, Area units ft^2.
    Alpha = Alpha / 57.2958; //convert Alpha into rad.
    V_ref = V_ref * 1.46667; //converts V_ref into ft/s
    LiftForce = NormalForce * cos(Alpha) - AxialForce * sin(Alpha); // units of lbf
    DragForce = AxialForce * cos(Alpha) - NormalForce * sin(Alpha); // units of lbf
    LiftCoefficent = 2 * LiftForce * 32.174 / (Density * V_ref * V_ref * Area);
    DragCoefficent = 2 * DragForce * 32.174 / (Density * V_ref * V_ref * Area);
    //The next two equations are from the wind tunnel manufactuar and correct for the fact that the wind tunnel isn't infinite.
    CorrectedDragCoefficent = DragCoefficent + 0.02 * LiftCoefficent * LiftCoefficent;
    CorrectedAlpha = Alpha + 1.5 * LiftCoefficent / 57.2958; 
    // the equation for change in alpha was in degrees, so it's devided by the conversion to rads.
}


void instance::GetData(istream& input) // gets the data for a single instance from a file
{
    input >> Date;
    input >> Time;
    input >> q;
    input >> V_ref;
    input >> Alpha;
    input >> NormalForce;
    input >> AxialForce;
    input >> Moment;
    input >> P;
    input >> Orientation;
    input.ignore(256, '\n'); // discards any notes that may exist and moves to the next line.
    return;
}


void instance::WriteData(ofstream& output, int spacing) //outputs the data for a single instance to a file. Might add more to allow uset to choose what data to output
{
    output << setw(spacing) << V_ref << ',';
    output << setw(spacing) << LiftForce << ',';
    output << setw(spacing) << DragForce << ',';
    output << setw(spacing) << LiftCoefficent << ',';
    output << setw(spacing) << DragCoefficent << ',';
    output << setw(spacing) << Alpha << ',';
    output << setw(spacing) << CorrectedAlpha << ',';
    output << setw(spacing) << CorrectedDragCoefficent << endl;
    return;
}


void instance::SetZero() //Sets all relavent values to zero
{
    q = 0;
    V_ref = 0;
    Alpha = 0;
    NormalForce = 0;
    AxialForce = 0;
    Moment = 0;
    P = 0;
}


void OutputData(vector<instance>& DataPoints) //this function deals with the output file stream and calling the WriteData function for each instance in the vector.
{
    //Get the requested filename from user and open it for writing.
    string FileName;
    cout << "What file name do you want for the data?\n";
    cin >> FileName;
    ofstream OutputFile;
    OutputFile.open(FileName);
    if (OutputFile.fail()) // Check if file actually opens.
    {
        cout << "Failed to open file.\n";
        return;
    }

    int spacing = 15; // this is to allow for easy formating of the output file during testing.
  
    // output headers for the data.
    OutputFile << setw(spacing) << "V_ref" << ',';
    OutputFile << setw(spacing) << "LF" << ',' << setw(spacing) << "DF" << ',';
    OutputFile << setw(spacing) << "CL" << ',' << setw(spacing) << "Cd" << ',';
    OutputFile << setw(spacing) << "A" << ',' << setw(spacing) << "A(Cor)" << ',';
    OutputFile << setw(spacing) << "CD(Cor)" << endl;
    OutputFile << setw(spacing) << "ft/s" << ',';
    OutputFile << setw(spacing) << "(lbf)" << ',' << setw(spacing) << "(lbf)" << ',';
    OutputFile << setw(spacing) << "" << ',' << setw(spacing) << "" << ',';
    OutputFile << setw(spacing) << "Rad" << ',' << setw(spacing) << "Rad" << endl;

    instance temp;

    for (int x = 0; x < DataPoints.size(); x++) // calls the WriteData function for each instance in the vector.
    {
        temp = DataPoints[x];
        temp.WriteData(OutputFile, spacing);
    }
    OutputFile.close();
    return;
}


void InputData(vector<instance>& DataPoints)
{
    //Get file name from user and open file
    string name; 
    cout << "Please input the name of the file to be analyzed, no whitespace.\n";
    cin >> name;
    ifstream InputData;
    InputData.open(name);
    if (InputData.fail())
    {
        cout << "Failed to open file.\n";
        return;
    }

    for (int x = 0; x < 9; x++) //ignores the first nine lines of header text
    {
        InputData.ignore(10000, '\n');
    }

    instance temp;
    instance lasttemp; 
    lasttemp.SetZero(); // this variable lets us check to see if we're taking the same data point twice
    int num = 0;
    while (InputData.eof() == false) //Calls GetData for every line of text in the file and adds them to vector DataPoints
    {
        temp.GetData(InputData);
        if (InputData.eof() == false)
        {
            DataPoints.push_back(temp);
        }
        num++;
    }

    InputData.close();
    return;
}


void AverageData(vector<instance>& DataPoints, vector<instance>& DataPointsAveraged) //Takes several data points from DataPoints that were taken at the same time and averages them into a single value in DataPointsAveraged
{
    //this section gets the number of instances that need to be averaged together from the user.
    int PointNum = 1; // PointNum is the number of data points we're taking from DataPoints and averaging into DataPointsAveraged
    //cout << "How many data points were taken at each angle and need to be averaged together?\n";
    //cin >> PointNum;

    if (DataPoints.size() % PointNum != 0) // Check if the number given devides evenly into the number of data points. If not, get a new number.
    {
        cout << "That number doesn't work with your data.\n";
        while (DataPoints.size() % PointNum != 0)
        {
            cout << "Please check you data and reenter the number of points to be averaged.\n";
            cin >> PointNum;
        }
    }

    int NumAvgDataPoints = 0;
    instance ToAvgPoints;
    while(NumAvgDataPoints != DataPoints.size() / PointNum)
    {
        ToAvgPoints.SetZero(); //sets all relavent variabls to zero.
        for (int x = 0; x < PointNum; x++)// Add up all the variables in the next PointNum of DataPoints
        {
            instance temp = DataPoints[NumAvgDataPoints * PointNum + x];
            ToAvgPoints.V_ref += temp.V_ref; // do for all variables you want to average.
            ToAvgPoints.Alpha += temp.Alpha;
            ToAvgPoints.NormalForce += temp.NormalForce;
            ToAvgPoints.AxialForce += temp.AxialForce;
        }
        ToAvgPoints.V_ref = ToAvgPoints.V_ref / PointNum;
        ToAvgPoints.Alpha = ToAvgPoints.Alpha / PointNum;
        ToAvgPoints.NormalForce = ToAvgPoints.NormalForce / PointNum;
        ToAvgPoints.AxialForce = ToAvgPoints.AxialForce / PointNum;

        DataPointsAveraged.push_back(ToAvgPoints); // Add the averaged instance to DataPointsAveraged
        NumAvgDataPoints++;
    }
    return;
}


void Compute(vector<instance>& AvgData) // Gets Density and area from user and calls compute for each data point in AvgData
{
    double Density=0.07186;
    double Area=0.412;
    //cout << "Please input the density of air you'd like to use. Units of lbm/ft^3" << endl;
    //cin >> Density;
    //cout << "Please input the area of your wing. Units of ft^2." << endl;
    //cin >> Area;
    for (int x = 0; x < AvgData.size(); x++)
    {
        instance temp = AvgData[x];
        temp.Compute(Density, Area);
        AvgData[x] = temp;
    }
}