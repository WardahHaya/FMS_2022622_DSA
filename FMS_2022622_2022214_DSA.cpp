#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <cstring>

// Constants
#define MAXCITY 30
#define MAXPASSENGERS 100

// Forward declarations
struct FlightType;
struct CityListType;
struct ReservationType;

// Function prototypes
void ShowAllCities();
void ShowDeparturesForCity(const std::vector<FlightType>& flights, const char* cityName);
void ShowArrivalsForCity(const std::vector<FlightType>& flights, const char* cityName);
void ShowCitiesReachableFrom(const std::vector<FlightType>& flights, const char* startCity);
void ShowShortestPath(const std::vector<FlightType>& flights, const char* startCity, const char* endCity);
void FindRoute(const std::vector<FlightType>& flights, const char* startCity, const char* endCity);
void MakeReservation(const std::vector<FlightType>& flights, const std::string& firstName, const std::string& lastName, const char* startCity, const char* endCity);
void PrintPassengerSchedule(const std::string& passengerName);
void DeletePassengerReservation(const std::string& passengerName);
void PrintPassengersForFlight(const std::vector<FlightType>& flights, int flightNo);

// Structure for FlightType
struct FlightType {
    int FlightNo;
    const char* startCity;
    int timeDepart;
    const char* endCity;
    int timeArrival;
    FlightType* nextDeparture; /* next departure node from this city */
    FlightType* nextArrival;   /* next arrival node to this city */
};

// Structure for CityListType
struct CityListType {
    const char* cityName;
    FlightType* nextDeparture; /* first departure from this city */
    FlightType* nextArrival;   /* first arrival to this city */
};

// Structure for ReservationType
struct ReservationType {
    int reservationNo;
    std::string passengerName;
    const char* startCity;
    const char* endCity;
    int flightNo;
    int departureTime;
    int arrivalTime;
};

// Global arrays
CityListType cityList[MAXCITY];
FlightType flightList[MAXCITY];
ReservationType reservations[MAXPASSENGERS];
int reservationCounter = 0;

// Function to show all cities
void ShowAllCities() {
    std::cout << "List of all cities serviced by the airline:\n";
    for (int i = 0; i < MAXCITY; ++i) {
        if (cityList[i].cityName != nullptr) {
            std::cout << cityList[i].cityName << std::endl;
        }
    }
    std::cout << std::endl;
}

// Function to display flight departures for a city
void ShowDeparturesForCity(const std::vector<FlightType>& flights, const char* cityName) {
    std::vector<FlightType> departures;
    for (const auto& flight : flights) {
        if (strcmp(flight.startCity, cityName) == 0) {
            departures.push_back(flight);
        }
    }
    std::sort(departures.begin(), departures.end(), [](const FlightType& a, const FlightType& b) {
        return a.timeDepart < b.timeDepart;
    });
    std::cout << "List of flight departures for " << cityName << ":\n";
    std::cout << "----------------------------------------\n";
    for (const auto& departure : departures) {
        std::cout << "Flight No: " << departure.FlightNo
                  << ", Departure Time: " << departure.timeDepart
                  << ", Arrival City: " << departure.endCity << std::endl;
    }
    std::cout << std::endl;
}

// Function to display flight arrivals for a city
void ShowArrivalsForCity(const std::vector<FlightType>& flights, const char* cityName) {
    std::vector<FlightType> arrivals;
    for (const auto& flight : flights) {
        if (strcmp(flight.endCity, cityName) == 0) {
            arrivals.push_back(flight);
        }
    }
    std::sort(arrivals.begin(), arrivals.end(), [](const FlightType& a, const FlightType& b) {
        return a.timeArrival < b.timeArrival;
    });
    std::cout << "List of flight arrivals for " << cityName << ":\n";
    std::cout << "--------------------------------------\n";
    for (const auto& arrival : arrivals) {
        std::cout << "Flight No: " << arrival.FlightNo
                  << ", Arrival Time: " << arrival.timeArrival
                  << ", Departure City: " << arrival.startCity << std::endl;
    }
    std::cout << std::endl;
}

// Function to show cities reachable from a particular city
void ShowCitiesReachableFrom(const std::vector<FlightType>& flights, const char* startCity) {
    std::vector<const char*> reachableCities;
    for (const auto& flight : flights) {
        if (strcmp(flight.startCity, startCity) == 0) {
            reachableCities.push_back(flight.endCity);
        }
    }
    std::sort(reachableCities.begin(), reachableCities.end());
    auto last = std::unique(reachableCities.begin(), reachableCities.end());
    reachableCities.erase(last, reachableCities.end());
    std::cout << "Cities reachable from " << startCity << ":\n";
    std::cout << "-------------------------------\n";
    for (const auto& city : reachableCities) {
        std::cout << city << std::endl;
    }
    std::cout << std::endl;
}

// Function to find the shortest path between two cities using BFS
bool FindShortestPath(const std::vector<FlightType>& flights, const char* startCity, const char* endCity, std::vector<const char*>& shortestPath) {
    std::queue<const char*> bfsQueue;
    std::unordered_map<const char*, const char*> parentMap;

    bfsQueue.push(startCity);
    parentMap[startCity] = nullptr;

    while (!bfsQueue.empty()) {
        const char* currentCity = bfsQueue.front();
        bfsQueue.pop();

        for (const auto& flight : flights) {
            if (strcmp(flight.startCity, currentCity) == 0) {
                const char* nextCity = flight.endCity;
                if (parentMap.find(nextCity) == parentMap.end()) {
                    bfsQueue.push(nextCity);
                    parentMap[nextCity] = currentCity;
                }
            }
        }
    }

    shortestPath.clear();
    const char* current = endCity;
    while (current != nullptr) {
        shortestPath.push_back(current);
        current = parentMap[current];
    }

    std::reverse(shortestPath.begin(), shortestPath.end());

    return !shortestPath.empty();
}

// Function to show the list of cities in the shortest path between any two cities
void ShowShortestPath(const std::vector<FlightType>& flights, const char* startCity, const char* endCity) {
    std::vector<const char*> shortestPath;
    if (FindShortestPath(flights, startCity, endCity, shortestPath)) {
        std::cout << "Shortest path from " << startCity << " to " << endCity << ":\n";
        for (const auto& city : shortestPath) {
            std::cout << city << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No path found between " << startCity << " and " << endCity << ".\n";
    }
}

// Placeholder for FindRoute function
void FindRoute(const std::vector<FlightType>& flights, const char* startCity, const char* endCity) {
    // Your implementation here
    // This function should find and display a route between two cities
    std::cout << "Functionality for finding a route is not implemented yet.\n";
}

// Function to make a reservation
void MakeReservation(const std::vector<FlightType>& flights, const std::string& firstName, const std::string& lastName, const char* startCity, const char* endCity) {
    // Find the first available reservation slot
    if (reservationCounter < MAXPASSENGERS) {
        ReservationType reservation;
        reservation.reservationNo = ++reservationCounter;
        reservation.passengerName = firstName + " " + lastName;
        reservation.startCity = startCity;
        reservation.endCity = endCity;

        // Find the first flight available for the reservation
        for (const auto& flight : flights) {
            if (strcmp(flight.startCity, startCity) == 0 && strcmp(flight.endCity, endCity) == 0) {
                reservation.flightNo = flight.FlightNo;
                reservation.departureTime = flight.timeDepart;
                reservation.arrivalTime = flight.timeArrival;
                break;
            }
        }

        // Add the reservation to the array
        reservations[reservationCounter - 1] = reservation;

        std::cout << "Reservation successful. Reservation No: " << reservation.reservationNo << std::endl;
    } else {
        std::cout << "No available reservation slots.\n";
    }
}

// Function to print a passenger's reservation schedule
void PrintPassengerSchedule(const std::string& passengerName) {
    for (const auto& reservation : reservations) {
        if (reservation.passengerName == passengerName) {
            std::cout << "Reservation No: " << reservation.reservationNo
                      << ", Passenger Name: " << reservation.passengerName
                      << ", Departure City: " << reservation.startCity
                      << ", Arrival City: " << reservation.endCity
                      << ", Flight No: " << reservation.flightNo
                      << ", Departure Time: " << reservation.departureTime
                      << ", Arrival Time: " << reservation.arrivalTime << std::endl;
            return;
        }
    }
    std::cout << "Passenger not found or has no reservations.\n";
}

// Function to delete a passenger's reservation
void DeletePassengerReservation(const std::string& passengerName) {
    for (int i = 0; i < reservationCounter; ++i) {
        if (reservations[i].passengerName == passengerName) {
            // Shift remaining reservations to fill the gap
            for (int j = i; j < reservationCounter - 1; ++j) {
                reservations[j] = reservations[j + 1];
            }
            --reservationCounter;
            std::cout << "Reservation deleted for passenger " << passengerName << std::endl;
            return;
        }
    }
    std::cout << "Passenger not found or has no reservations.\n";
}

// Function to print a list of passengers for a particular flight (in order of last name)
void PrintPassengersForFlight(const std::vector<FlightType>& flights, int flightNo) {
    std::vector<ReservationType> passengersForFlight;
    for (const auto& reservation : reservations) {
        for (const auto& flight : flights) {
            if (reservation.flightNo == flightNo && strcmp(flight.startCity, reservation.startCity) == 0 && strcmp(flight.endCity, reservation.endCity) == 0) {
                passengersForFlight.push_back(reservation);
                break;
            }
        }
    }
    std::sort(passengersForFlight.begin(), passengersForFlight.end(), [](const ReservationType& a, const ReservationType& b) {
        return a.passengerName < b.passengerName;
    });

    std::cout << "List of passengers for Flight No " << flightNo << " (in order of last name):\n";
    std::cout << "---------------------------------------------------------------------------\n";
    for (const auto& passenger : passengersForFlight) {
        std::cout << "Reservation No: " << passenger.reservationNo
                  << ", Passenger Name: " << passenger.passengerName
                  << ", Departure City: " << passenger.startCity
                  << ", Arrival City: " << passenger.endCity << std::endl;
    }
    std::cout << std::endl;
}

// Function to dynamically generate city options based on available flights
std::vector<std::string> GenerateCityOptions(const std::vector<FlightType>& flights) {
    std::vector<std::string> cityOptions;
    for (const auto& flight : flights) {
        cityOptions.push_back(flight.startCity);
        cityOptions.push_back(flight.endCity);
    }
    std::sort(cityOptions.begin(), cityOptions.end());
    auto last = std::unique(cityOptions.begin(), cityOptions.end());
    cityOptions.erase(last, cityOptions.end());
    return cityOptions;
}

int main() {
    // Example flight data
    std::vector<FlightType> flights = {
        {1, "Karachi", 830, "Islamabad", 950},
        {2, "Karachi", 930, "Lahore", 1130},
        {3, "Lahore", 1200, "Islamabad", 1400},
        {4, "Islamabad", 1400, "Karachi", 1600},
        {5, "Lahore", 1100, "Karachi", 1230},
        {6, "Islamabad", 1500, "Lahore", 1630}
        // Add more flight data as needed
    };

    // Example city data
    cityList[0] = {"Karachi", nullptr, nullptr};
    cityList[1] = {"Lahore", nullptr, nullptr};
    cityList[2] = {"Islamabad", nullptr, nullptr};
    // Add more city data as needed

    // Choose the operation to perform
    int choice;
    do {
        std::cout << "1. Show all cities\n"
                  << "2. Show departures for a city\n"
                  << "3. Show arrivals for a city\n"
                  << "4. Show cities reachable from a city\n"
                  << "5. Show shortest path between two cities\n"
                  << "6. Find a route between two cities\n"
                  << "7. Make a reservation\n"
                  << "8. Print a passenger's reservation schedule\n"
                  << "9. Delete a passenger's reservation\n"
                  << "10. Print a list of passengers for a particular flight (in order of last name)\n"
                  << "0. Exit\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: // Show all cities
                ShowAllCities();
                break;
            case 2: // Show departures for a city
                {
                    std::string startCity;
                    std::cout << "Enter the departure city: ";
                    std::cin >> startCity;
                    ShowDeparturesForCity(flights, startCity.c_str());
                }
                break;
            case 3: // Show arrivals for a city
                {
                    std::string endCity;
                    std::cout << "Enter the arrival city: ";
                    std::cin >> endCity;
                    ShowArrivalsForCity(flights, endCity.c_str());
                }
                break;
            case 4: // Show cities reachable from a city
                {
                    std::string startCity;
                    std::cout << "Enter the starting city: ";
                    std::cin >> startCity;
                    ShowCitiesReachableFrom(flights, startCity.c_str());
                }
                break;
            case 5: // Show shortest path between two cities
                {
                    std::string startCity, endCity;
                    std::cout << "Enter the starting city: ";
                    std::cin >> startCity;
                    std::cout << "Enter the destination city: ";
                    std::cin >> endCity;
                    ShowShortestPath(flights, startCity.c_str(), endCity.c_str());
                }
                break;
            case 6: // Find a route between two cities
                {
                    std::string startCity, endCity;
                    std::cout << "Enter the starting city: ";
                    std::cin >> startCity;
                    std::cout << "Enter the destination city: ";
                    std::cin >> endCity;
                    FindRoute(flights, startCity.c_str(), endCity.c_str());
                }
                break;
            case 7: // Make a reservation
                {
                    std::string firstName, lastName, startCity, endCity;
                    std::cout << "Enter passenger first name: ";
                    std::cin >> firstName;
                    std::cout << "Enter passenger last name: ";
                    std::cin >> lastName;
                    std::cout << "Enter departure city: ";
                    std::cin >> startCity;
                    std::cout << "Enter arrival city: ";
                    std::cin >> endCity;
                    MakeReservation(flights, firstName, lastName, startCity.c_str(), endCity.c_str());
                }
                break;
            case 8: // Print a passenger's reservation schedule
                {
                    std::string passengerName;
                    std::cout << "Enter passenger name: ";
                    std::cin.ignore(); // Ignore newline character left in the stream
                    std::getline(std::cin, passengerName);
                    PrintPassengerSchedule(passengerName);
                }
                break;
            case 9: // Delete a passenger's reservation
                {
                    std::string passengerName;
                    std::cout << "Enter passenger name: ";
                    std::cin.ignore(); // Ignore newline character left in the stream
                    std::getline(std::cin, passengerName);
                    DeletePassengerReservation(passengerName);
                }
                break;
            case 10: // Print a list of passengers for a particular flight (in order of last name)
                {
                    int flightNo;
                    std::cout << "Enter the flight number: ";
                    std::cin >> flightNo;
                    PrintPassengersForFlight(flights, flightNo);
                }
                break;
            case 0: // Exit
                std::cout << "Exiting program.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }

    } while (choice != 0);

    return 0;
}
