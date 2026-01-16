#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iomanip> // needed for std::fixed and std::setprecision

enum class RiskLVL // LOW = 0, MEDIUM = 1, HIGH = 2
{
    LOW,
    MEDIUM,
    HIGH
};

class Observation{ // price, volume and t_stamp (time), with method .dollar_value = price * volume.
private:
    double price; // price of 1 unit of the traded asset at the time of the trade
    double volume; // how much of the asset was exchanged
    long t_stmp; // when this exchange occured
public:
    Observation(double p,double v,long t) // efficient constructor of instance of class
    :price(p),volume(v),t_stmp(t){}

    double get_price() const { return price;} // const added to allow printing const Observations
    double get_vol() const { return volume;}
    long get_time() const { return t_stmp;}
    
    double dollar_value() const // the dollar value of the trade which occurred
    {
        return price * volume;
    }
};

class SignalSeries{ // vector of observations (series) indexed by .t_smp, .in(&observation) method, and .cardinality()
    std::vector<Observation> series;
    //Vectors own heap memory containing entries by design.
    //Heap memory is safe in C++ when it is owned by an object whose destructor frees it.
    //Destructor is called after return value passed into register in scope in which its envoked.

public:
    const std::vector<Observation>& get_vec() const{ // changed to return const reference instead of copy
        return series;
    }
    
    void in(const Observation& obs) // put in new obs, use reference to avoid copying
    {
        series.push_back(obs);
    }
    double risk_multiplier(RiskLVL level) const
    {
        switch (level){
            case RiskLVL::LOW: 
            return 0.35;
            
            case RiskLVL::MEDIUM: 
            return 0.67;
            
            case RiskLVL::HIGH: 
            return 1.0;

            default: return 0.0;
        }
    }
    const double weighted_signal(RiskLVL level) const
    {
        double risk = risk_multiplier(level);
        double sum_of_vals = 0.0;
        for(const Observation &s: series) // no modifying, use address to avoid copying entire series at run time
        {
            sum_of_vals += s.dollar_value();
        }
        return risk * sum_of_vals;
        
    }

    std::size_t cardinality() const
    {
        return series.size();
    }
};

// Now we will create an operator which we can use to express each Observation (printing or writing to a file etc)
std::ostream& operator<<(std::ostream& os, const Observation& obs) { // changed to const Observation&
    // return type is out-stream pointer/ref, an ostream is a Class which defines how data are sent to other things
    // each instance of ostream, like cout, has a memory address defined by c++, cout has the address of your terminal screen
    // we reference(point to) this address with ostream& os. We will send data which has addy Observation& obs to that of os.
    // here os is any stream to which we can write to, like cout, or any other pre-existing buffer. (e.g writing to a file with ofstream)
    // the function we're defining is the binary operator <<
    // operator<< is non-commutative, unlike operator+(a,b)=operator+(b,a). It is directional.
    // Use 'os' exactly like 'cout' to format the string, cout is an instance of the class ostream
    os << std::fixed << std::setprecision(2) // added fixed and precision to fix weird price printing
       << "Price: $" << obs.get_price() 
       << " | Volume: " << obs.get_vol() 
       << " | Timestamp: " 
       << std::setw(6) << std::setfill('0') << obs.get_time(); // leading zeros for timestamp kept with "setw(6) and setfill"
    
    // Return the stream so the next << can use it, i.e we can send multiple Observations through
    return os; 
};



/* 
    - Example usage below in main(){}
    - Adds 3 signals intro SignalSeries.series
    - Expresses the current contents of this SignalSeries vector
*/
using std::cout, std::cin, std::endl;
int main()
{
    cout << "ILLUSTRATION: LOGGING BITCOIN/US DOLLAR TRANSACTIONS\n__________________________________________________\n";
    
    // timestamps fixed (removed leading zeros to avoid octal)
    Observation x1(96601.02,1.23,4500);
    Observation x2(96601.08,0.56,4501);
    Observation x3(96601.10,0.22,4502);
    Observation x4(96601.09,1.57,4503);
    SignalSeries VecX;
    VecX.in(x1);
    VecX.in(x2);
    VecX.in(x3);
    VecX.in(x4);

    for(const Observation &obs: VecX.get_vec()){
        cout << obs << endl;

    }

}
