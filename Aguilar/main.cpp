// ==========================================
//   CONCRETE MIX DESIGN CALCULATOR
//   CFP11 — Computer Fundamentals & Programming
//   Student: Aguilar, Vejay
//   Date: October 2026
// ==========================================

#include <iostream>
#include <iomanip>  // For neat output formatting
using namespace std;

// ====== FUNCTION 1: Calculate expected compressive strength ======
// Formula: f'c = A * (C/W - B)  — simplified empirical formula
// C/W = cement-water ratio
double calculateStrength(double cementRatio, double waterRatio) {
    double cwRatio = cementRatio / waterRatio;
    const double A = 14.0;   // Empirical constant
    const double B = 0.50;   // Constant for normal concrete
    double strength = A * (cwRatio - B);
    if (strength < 0) strength = 0;
    return strength;
}

// ====== FUNCTION 2: Validate mix proportions ======
bool isMixValid(double cement, double sand, double aggregate, double water, double targetStrength) {
    // All ratios must be positive
    if (cement <= 0 || sand <= 0 || aggregate <= 0 || water <= 0) {
        cout << "❌ Error: All ratios must be positive numbers." << endl;
        return false;
    }

    double wcRatio = water / cement;

    // Water-cement ratio limits (ACI standard range)
    if (wcRatio < 0.35) {
        cout << "⚠️  Warning: Water-cement ratio is very low — mix may be too dry." << endl;
    }
    if (wcRatio > 0.70) {
        cout << "❌ Error: Water-cement ratio (" << fixed << setprecision(2) << wcRatio
             << ") exceeds maximum 0.70 — mix will be too weak." << endl;
        return false;
    }

    // Check if mix can reach target strength
    double expectedStrength = calculateStrength(cement, water);
    if (expectedStrength < targetStrength) {
        cout << "⚠️  Warning: With these ratios, expected strength is "
             << fixed << setprecision(1) << expectedStrength
             << " MPa — below your target of " << targetStrength << " MPa." << endl;
        cout << "   Try increasing cement ratio or decreasing water ratio." << endl;
    }

    return true;
}

// ====== FUNCTION 3: Calculate material quantities ======
// Returns total weight of each material for given volume (cubic meters)
void calculateQuantities(double volume, double cement, double sand, double aggregate, double water,
                         double &cementKg, double &sandKg, double &aggregateKg, double &waterLiters) {
    // Total ratio sum
    double totalRatio = cement + sand + aggregate;

    // Typical concrete density = 2400 kg/m³
    const double density = 2400.0;
    double totalWeight = volume * density;

    // Calculate each material weight
    cementKg = (cement / totalRatio) * totalWeight;
    sandKg = (sand / totalRatio) * totalWeight;
    aggregateKg = (aggregate / totalRatio) * totalWeight;

    // Water: 1 kg = 1 liter
    double waterPerKgCement = water / cement;
    waterLiters = waterPerKgCement * cementKg;
}

// ====== FUNCTION 4: Calculate material costs ======
double calculateCost(double cementKg, double sandKg, double aggregateKg,
                     double cementPrice, double sandPrice, double aggregatePrice) {
    const double waterPricePerLiter = 0.05;  // Example cost ₱ or $ per liter
    double waterLiters = (waterPricePerLiter * 1); // placeholder — recalculate in main
    double total = cementKg * cementPrice / 50.0  // Cement often sold per 50kg bag
                + sandKg * sandPrice
                + aggregateKg * aggregatePrice;
    return total;
}

// ====== FUNCTION 5: Display final report ======
void displayReport(double targetStrength, double expectedStrength, double volume,
                   double cementKg, double sandKg, double aggregateKg, double waterLiters,
                   double totalCost) {
    cout << "\n==================================================" << endl;
    cout << "       CONCRETE MIX DESIGN REPORT" << endl;
    cout << "==================================================" << endl;
    cout << "Target Compressive Strength:  " << fixed << setprecision(1) << targetStrength << " MPa" << endl;
    cout << "Expected Strength:            " << fixed << setprecision(1) << expectedStrength << " MPa" << endl;
    cout << "Batch Volume:                 " << fixed << setprecision(2) << volume << " m³" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "MATERIAL QUANTITIES:" << endl;
    cout << "  Cement:      " << fixed << setprecision(1) << cementKg << " kg" << endl;
    cout << "  Sand:        " << fixed << setprecision(1) << sandKg << " kg" << endl;
    cout << "  Aggregate:   " << fixed << setprecision(1) << aggregateKg << " kg" << endl;
    cout << "  Water:       " << fixed << setprecision(1) << waterLiters << " liters" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "Estimated Total Cost:         " << fixed << setprecision(2) << totalCost << endl;
    cout << "==================================================\n" << endl;
}

// ====== MAIN PROGRAM ======
int main() {
    cout << "==================================================" << endl;
    cout << "    CONCRETE MIX DESIGN CALCULATOR" << endl;
    cout << "==================================================" << endl;
    cout << "Based on ACI proportioning principles\n" << endl;

    // 1. Get inputs from user
    double targetStrength, volume;
    double cementRatio, sandRatio, aggregateRatio, waterRatio;
    double cementPrice, sandPrice, aggregatePrice;

    cout << "Enter desired compressive strength (MPa): ";
    cin >> targetStrength;

    cout << "Enter batch volume (m³): ";
    cin >> volume;

    cout << "\nEnter mix proportions (parts by weight):" << endl;
    cout << "  Cement ratio:    ";
    cin >> cementRatio;
    cout << "  Sand ratio:      ";
    cin >> sandRatio;
    cout << "  Aggregate ratio: ";
    cin >> aggregateRatio;
    cout << "  Water ratio:     ";
    cin >> waterRatio;

    cout << "\nEnter material costs:" << endl;
    cout << "  Cement (per 50kg bag): ";
    cin >> cementPrice;
    cout << "  Sand (per kg):         ";
    cin >> sandPrice;
    cout << "  Aggregate (per kg):    ";
    cin >> aggregatePrice;

    // 2. Validate inputs
    if (targetStrength <= 0 || targetStrength > 100) {
        cout << "❌ Error: Target strength must be between 0 and 100 MPa." << endl;
        return 1;
    }
    if (volume <= 0) {
        cout << "❌ Error: Volume must be greater than 0." << endl;
        return 1;
    }
    if (!isMixValid(cementRatio, sandRatio, aggregateRatio, waterRatio, targetStrength)) {
        return 1;
    }

    // 3. Perform calculations
    double cementKg, sandKg, aggregateKg, waterLiters;
    calculateQuantities(volume, cementRatio, sandRatio, aggregateRatio, waterRatio,
                         cementKg, sandKg, aggregateKg, waterLiters);

    double expectedStrength = calculateStrength(cementRatio, waterRatio);

    // Recalculate cost with correct water volume
    const double waterPricePerLiter = 0.05;
    double totalCost = cementKg * cementPrice / 50.0
                     + sandKg * sandPrice
                     + aggregateKg * aggregatePrice
                     + waterLiters * waterPricePerLiter;

    // 4. Display results
    displayReport(targetStrength, expectedStrength, volume,
                  cementKg, sandKg, aggregateKg, waterLiters, totalCost);

    return 0;
}
