#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <regex>
#include <fstream>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"


// Passenger class
class Passenger
{
private:
    std::string name;
    int age;
    std::string gender;
    std::string coachPreferred;
    std::string berthPreferred;
    std::string bookingDate;
    std::string travelingDate;
    std::string status;
    std::string fromStation;
    std::string toStation;
    std::string trainName;
    std::string trainNumber;
    std::string pnr;
    int seatsBooked;
    std::string seatNumber;

public:
    Passenger(const std::string &name, int age, const std::string &gender, const std::string &coachPreferred, const std::string &berthPreferred,
              const std::string &bookingDate, const std::string &travelingDate, const std::string &status, const std::string &fromStation,
              const std::string &toStation, const std::string &trainName, const std::string &trainNumber, const std::string &pnr, int seatsBooked, const std::string &seatNumber)
        : name(name), age(age), gender(gender), coachPreferred(coachPreferred), berthPreferred(berthPreferred),
          bookingDate(bookingDate), travelingDate(travelingDate), status(status), fromStation(fromStation),
          toStation(toStation), trainName(trainName), trainNumber(trainNumber), pnr(pnr), seatsBooked(seatsBooked), seatNumber(seatNumber) {}

    // Getter methods
    std::string getName() const { return name; }
    int getAge() const { return age; }
    std::string getGender() const { return gender; }
    std::string getCoachPreferred() const { return coachPreferred; }
    std::string getBerthPreferred() const { return berthPreferred; }
    std::string getBookingDate() const { return bookingDate; }
    std::string getTravelingDate() const { return travelingDate; }
    std::string getStatus() const { return status; }
    std::string getFromStation() const { return fromStation; }
    std::string getToStation() const { return toStation; }
    std::string getTrainName() const { return trainName; }
    std::string getTrainNumber() const { return trainNumber; }
    std::string getPnr() const { return pnr; }
    int getSeatsBooked() const { return seatsBooked; }
    std::string getSeatNumber() const { return seatNumber; }

    // Serialization
    std::string toString() const
    {
        std::ostringstream oss;
        oss << name << ","
            << age << ","
            << gender << ","
            << coachPreferred << ","
            << berthPreferred << ","
            << bookingDate << ","
            << travelingDate << ","
            << status << ","
            << fromStation << ","
            << toStation << ","
            << trainName << ","
            << trainNumber << ","
            << pnr << ","
            << seatsBooked << ","
            << seatNumber;
        return oss.str();
    }

    static Passenger fromString(const std::string &str)
    {
        std::istringstream iss(str);
        std::string token;
        std::getline(iss, token, ',');
        std::string name = token;
        std::getline(iss, token, ',');
        int age = std::stoi(token);
        std::getline(iss, token, ',');
        std::string gender = token;
        std::getline(iss, token, ',');
        std::string coachPreferred = token;
        std::getline(iss, token, ',');
        std::string berthPreferred = token;
        std::getline(iss, token, ',');
        std::string bookingDate = token;
        std::getline(iss, token, ',');
        std::string travelingDate = token;
        std::getline(iss, token, ',');
        std::string status = token;
        std::getline(iss, token, ',');
        std::string fromStation = token;
        std::getline(iss, token, ',');
        std::string toStation = token;
        std::getline(iss, token, ',');
        std::string trainName = token;
        std::getline(iss, token, ',');
        std::string trainNumber = token;
        std::getline(iss, token, ',');
        std::string pnr = token;
        std::getline(iss, token, ',');
        int seatsBooked = std::stoi(token);
        std::getline(iss, token, ',');
        std::string seatNumber = token;
        return Passenger(name, age, gender, coachPreferred, berthPreferred, bookingDate, travelingDate, status, fromStation, toStation, trainName, trainNumber, pnr, seatsBooked, seatNumber);
    }
};


class Train
{
private:
    std::map<std::string, int> coachFares;
    std::map<std::string, std::map<std::string, int>> trainFares;
    std::vector<Passenger> passengers;
    std::set<std::string> usedPnrs;
    std::string adminPassword;
    std::map<std::pair<std::string, std::string>, int> distanceMap;

    std::string getCurrentDate() const
    {
        std::time_t now = std::time(0);
        std::tm *ltm = std::localtime(&now);
        std::ostringstream dateStream;
        dateStream << std::setfill('0') << std::setw(2) << ltm->tm_mday << "-"
                   << std::setfill('0') << std::setw(2) << (1 + ltm->tm_mon) << "-"
                   << 1900 + ltm->tm_year;
        return dateStream.str();
    }

    bool isValidDate(const std::string &date) const
    {
        std::regex datePattern("\\d{2}-\\d{2}-\\d{4}");
        if (!std::regex_match(date, datePattern))
            return false;

        int day, month, year;
        std::sscanf(date.c_str(), "%d-%d-%d", &day, &month, &year);

        if (month < 1 || month > 12 || day < 1 || day > 31)
            return false;
        if (month == 2 && day > 29)
            return false;
        if (month == 2 && day > 28 && !((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
            return false;
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
            return false;

        return true;
    }

    std::string generateRandomPnr()
    {
        std::string pnr;
        do
        {
            pnr = std::to_string(rand() % 900000000 + 100000000); // 9-digit number
        } while (usedPnrs.find(pnr) != usedPnrs.end()); // Ensure uniqueness
        usedPnrs.insert(pnr);
        return pnr;
    }

    std::string generateRandomSeatNumber() const
    {
        char seatPrefix = 'A' + rand() % 4; // Random seat class prefix (A-D)
        int seatNumber = rand() % 100 + 1;  // Random seat number between 1 and 100
        std::ostringstream seatStream;
        seatStream << seatPrefix << seatNumber;
        return seatStream.str();
    }

    int calculateDistance(const std::string &fromStation, const std::string &toStation) const
    {
        auto it = distanceMap.find({fromStation, toStation});
        if (it != distanceMap.end())
        {
            return it->second;
        }
        // If direct distance is not available, try reverse
        it = distanceMap.find({toStation, fromStation});
        if (it != distanceMap.end())
        {
            return it->second;
        }
        // Default distance if not found
        return 0;
    }

    int generateRandomFare() const
    {
        return rand() % 500 + 100; // Random fare between 100 and 600 units
    }

    int calculateSingleSeatFare(const std::string &trainName, const std::string &coach, const std::string &fromStation, const std::string &toStation) const
    {
        auto trainIt = trainFares.find(trainName);
        if (trainIt != trainFares.end())
        {
            auto coachIt = trainIt->second.find(coach);
            if (coachIt != trainIt->second.end())
            {
                int baseFare = coachIt->second;
                int distance = calculateDistance(fromStation, toStation);
                return baseFare + distance;
            }
        }
        return generateRandomFare();
    }

    void loadData()
    {
        std::ifstream inFile("passengerData.txt");
        std::string line;
        while (std::getline(inFile, line))
        {
            if (!line.empty())
            {
                passengers.push_back(Passenger::fromString(line));
            }
        }
        inFile.close();
    }

    void saveData() const
    {
        std::ofstream outFile("passengerData.txt");
        for (const auto &p : passengers)
        {
            outFile << p.toString() << "\n";
        }
        outFile.close();
    }

public:
    Train(const std::string &adminPass) : adminPassword(adminPass)
    {
        coachFares["Sleeper"] = 150;
        coachFares["AC"] = 500;
        coachFares["First Class"] = 1000;
        // Add more coaches if needed

        // Initialize some train-specific fares (example)
        trainFares["Express"]["Sleeper"] = 150;
        trainFares["Express"]["AC"] = 500;
        trainFares["Express"]["First Class"] = 1000;
        trainFares["Superfast"]["Sleeper"] = 200;
        trainFares["Superfast"]["AC"] = 600;
        trainFares["Superfast"]["First Class"] = 1200;

        // Initialize distance map (example)
        distanceMap[{"CityA", "CityB"}] = 100;
        distanceMap[{"CityB", "CityC"}] = 200;
        distanceMap[{"CityA", "CityC"}] = 300;

        loadData(); // Load data from file
    }

    ~Train()
    {
        saveData(); // Save data to file on destruction
    }

    void bookTicket()
    {
        int n;
        std::string trainName, trainNumber, fromStation, toStation, travelingDate, coachPreferred, berthPreferred;

        std::cout << YELLOW << BOLD << "Enter the train name: " << RESET;
        std::cin.ignore();
        std::getline(std::cin, trainName);
        trainNumber = std::to_string(rand() % 9000 + 1000); // Random 4-digit number

        std::cout << YELLOW << BOLD << "Enter the source station: " << RESET;
        std::getline(std::cin, fromStation);
        std::cout << YELLOW << BOLD << "Enter the destination station: " << RESET;
        std::getline(std::cin, toStation);

        do
        {
            std::cout << YELLOW << BOLD << "Enter the traveling date (dd-mm-yyyy): " << RESET;
            std::getline(std::cin, travelingDate);
            if (!isValidDate(travelingDate))
            {
                std::cout << RED << BOLD << "Invalid date format. Please try again.\n"<< RESET;
            }
        } while (!isValidDate(travelingDate));

        std::cout << YELLOW<< BOLD << "Enter the number of seats to book: " << RESET;
        std::cin >> n;
        std::cin.ignore(); // Clear the newline character from the input buffer

        std::cout << YELLOW << BOLD << "Enter your preferred coach (Sleeper, AC, First Class): " << RESET;
        std::getline(std::cin, coachPreferred);
        std::cout << YELLOW << BOLD << "Enter your preferred berth (Upper, Middle, Lower): " << RESET;
        std::getline(std::cin, berthPreferred);

        for (int i = 0; i < n; ++i)
        {
            std::string name, gender, status;
            int age;

            std::cout << GREEN << BOLD << "Enter the name of passenger " << i + 1 << ": " << RESET;
            std::getline(std::cin, name);
            std::cout << GREEN<< BOLD << "Enter the age of passenger " << i + 1 << ": " << RESET;
            std::cin >> age;
            std::cin.ignore(); // Clear the newline character from the input buffer

            std::cout << GREEN << BOLD << "Enter the gender of passenger " << i + 1 << " (M/F/O): " << RESET;
            std::getline(std::cin, gender);

            std::string bookingDate = getCurrentDate();
            std::string pnr = generateRandomPnr();
            std::string seatNumber = generateRandomSeatNumber();
            status = "Confirmed"; // This can be extended to handle RAC/WL based on seat availability

            int fare = calculateSingleSeatFare(trainName, coachPreferred, fromStation, toStation);
            std::cout << CYAN << BOLD << "Fare for passenger " << i + 1 << ": " <<" INR "<< fare<<"\n" << RESET;

            passengers.emplace_back(name, age, gender, coachPreferred, berthPreferred, bookingDate, travelingDate, status, fromStation, toStation, trainName, trainNumber, pnr, 1, seatNumber);
        }
        saveData(); // Save the updated data
    }

    void cancelTicket()
    {
        std::string pnr;
        std::cout << BLUE << BOLD << "Enter the PNR number to cancel the ticket: " << RESET;
        std::cin.ignore();
        std::getline(std::cin, pnr);

        auto it = std::remove_if(passengers.begin(), passengers.end(), [&pnr](const Passenger &p)
                                 { return p.getPnr() == pnr; });

        if (it != passengers.end())
        {
            passengers.erase(it, passengers.end());
            std::cout << GREEN << BOLD << "Ticket with PNR " << pnr << " has been canceled successfully.\n"
                      << RESET;
        }
        else
        {
            std::cout << RED << BOLD << "No ticket found with PNR! " << pnr << ".\n"
                      << RESET;
        }
        saveData(); // Save the updated data
    }

    void passengerPanel()
    {
        int choice;
       std::cout <<CYAN<<BOLD<< "=======================================================================\n";
    std::cout <<CYAN<<BOLD<< "========================== Passenger Panel ===========================\n";
    std::cout<<CYAN<<BOLD << "=======================================================================\n"<<RESET;
    std::cout << RESET;
        std::cout << YELLOW << BOLD << "1. Book Ticket\n";
        std::cout << YELLOW << BOLD << "2. Cancel Ticket\n";
        std::cout << YELLOW << BOLD << "3. Back to Main Menu\n";
        std::cout << BLUE << BOLD << "Enter your choice: " << RESET;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            bookTicket();
            break;
        case 2:
            cancelTicket();
            break;
        case 3:
            return;
        default:
            std::cout << RED << BOLD << "Invalid choice! Please try again.\n"<< RESET;
            break;
        }
    }

    void adminPanel()
    {
        std::string password;
        std::cout << YELLOW<< BOLD << "Enter admin password: " << RESET;
        std::cin.ignore();
        std::getline(std::cin, password);

        if (password == adminPassword)
        {
            std::cout << GREEN << BOLD << "Login successful!\n"<< RESET;
           std::cout<<CYAN<<BOLD << "=======================================================================\n";
           std::cout <<CYAN<<BOLD<< "========================== Admin Panel ===========================\n";
           std::cout<<CYAN<<BOLD << "=======================================================================\n"<<RESET;
    std::cout << RESET;
            
            int n ;
            std::cout << YELLOW << BOLD << "Enter the no. of passenger data you want to print"<<RESET;
            std::cin >> n ;

            std::cout << CYAN << BOLD << "Passenger Details:\n"<< RESET;
            for (const auto &p : passengers)
            {
                for (int i = 1; i <= n; i++)
                {
                    std::cout <<RED<<BOLD<< "        ==========================passenger -" << i << "===================================\n"<<RESET;
                    std::cout << YELLOW << "Name: " << p.getName() << ", Age: " << p.getAge() << ", Gender: " << p.getGender() << ", Coach: " << p.getCoachPreferred()
                              << ", Berth: " << p.getBerthPreferred() << ", Booking Date: " << p.getBookingDate() << ", Traveling Date: " << p.getTravelingDate()
                              << ", Status: " << p.getStatus() << ", From: " << p.getFromStation() << ", To: " << p.getToStation() << ", Train Name: " << p.getTrainName()
                              << ", Train Number: " << p.getTrainNumber() << ", PNR: " << p.getPnr() << ", Seats Booked: " << p.getSeatsBooked()
                              << ", Seat Number: " << p.getSeatNumber() << "\n"<< RESET;
                    std::cout << "---------------------------------------------------------------------------------------------------------------------------------\n";
                }
                break;
            }
        }
        else
        {
            std::cout << RED << BOLD << "Incorrect password. Access denied.\n"
                      << RESET;
        }
    }
   

};

// Main function
int main()
{
    std::string adminPassword = "admin"; // Set a default password for simplicity
    Train railwaySystem(adminPassword);

    int choice;
    while (true)
    {
       std::cout<<CYAN<<BOLD << "=======================================================================\n";
           std::cout<<CYAN<<BOLD << "========================== Railway Reservation System ===========================\n";
           std::cout<<CYAN<<BOLD << "=======================================================================\n"<<RESET;
        std::cout << GREEN << BOLD << "1. Passenger Panel\n";
        std::cout << GREEN << BOLD << "2. Admin Panel\n";
        std::cout << GREEN << BOLD << "3. Exit\n";
        std::cout << BLUE << BOLD << "Enter your choice: " << RESET;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            railwaySystem.passengerPanel();
            break;
        case 2:
            railwaySystem.adminPanel();
            break;
        case 3:
            return 0;
        default:
            std::cout << RED << BOLD << "Invalid choice! Please try again.\n"
                      << RESET;
            break;
        }
    }
}
