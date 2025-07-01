#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <stdexcept>

using namespace std;

// Інтерполяція з файлу
double interpolate(const string& filename, double x) {
    ifstream file(filename);
    if (!file.is_open())
        throw runtime_error("Ne vdalosia vidkryty " + filename);

    double x1, y1, x2, y2;
    bool first = true;
    while (file >> x2 >> y2) {
        if (!first) {
            if (x >= x1 && x <= x2)
                return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
        }
        x1 = x2; y1 = y2;
        first = false;
    }
    throw runtime_error("Znachennia x ne znaideno u " + filename);
}

// Пошук значення для тексту
double getGtext(const string& filename, const string& text) {
    ifstream file(filename);
    if (!file.is_open())
        throw runtime_error("Ne vdalosia vidkryty " + filename);

    string word;
    double val;
    while (file >> word >> val) {
        if (word == text)
            return val;
    }
    return 0.0;
}

// Функція Qqn
double Qqn(double x, double y, double z, const string& dat1, const string& dat2) {
    double Ux = interpolate(dat1, x);
    double Uy = interpolate(dat1, y);
    double Tx = interpolate(dat2, x);
    double Tz = interpolate(dat2, z);
    return (z * Tz + y * Uy - Tx);
}

// Qnk
double Qnk(double x, double y, double z, const string& dat1, const string& dat2) {
    return Qqn(x, y, z, dat1, dat2) + Qqn(y, y, z, dat1, dat2) + Qqn(x, x, z, dat1, dat2);
}

// Rnk
double Rnk(double x, double y, double z, const string& dat1, const string& dat2) {
    return Qnk(x, y, z, dat1, dat2) + Qnk(y, y, z, dat1, dat2) + Qnk(x, x, z, dat1, dat2);
}

// funcr
double funcr(double x, double y, double z, const string& dat1, const string& dat2) {
    return Rnk(x, y, z, dat1, dat2);
}

// CTextf
double CTextf(const string& filename, string text, double x) {
    if (text.empty())
        return getGtext(filename, to_string(x));
    else if (x == 0)
        return getGtext(filename, text);
    else
        return getGtext(filename, text) - getGtext(filename, to_string(x));
}

// Y(x)
double Y(double x) {
    double val = 100 - x * x;
    if (val <= 0 || 1100 - x * x < 0)
        throw runtime_error("Nekorektne znachennia u Y(x)");
    return log(val);
}

// Variant
double Variant(double r, double k, double x, double y, double z, string text,
    const string& dat1, const string& dat2, const string& dat3) {
    double part1 = 0.8973 * k + 0.1027 * k;

    double f = funcr(x, y, z, dat1, dat2);
    double fTrr = r + k * r;
    double fR = k + 0.5 * fTrr;

    if (fR * fR - 4 * r < 0) k = 0;
    if (100 - r * r < 0 || 1100 - r * r < 0) k = 0;

    double ct = CTextf(dat3, text, x);
    return part1 + ct;
}

int main() {
    double r, k, x, y, z;
    string text;

    cout << "Vvedit r, k, x, y, z: ";
    cin >> r >> k >> x >> y >> z;
    cout << "Vvedit tekstovyi riadok: ";
    cin >> text;

    try {
        double res = Variant(r, k, x, y, z, text, "dat1.dat", "dat2.dat", "dat3.dat");
        cout << "Variant(r, k) = " << res << endl;
    }
    catch (exception& e) {
        cerr << "Pomylka: " << e.what() << endl;
    }

    return 0;
}
