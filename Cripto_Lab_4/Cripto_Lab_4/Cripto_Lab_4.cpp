#include<iostream>
#include<vector>
#include <cmath>
#include<random>

using namespace std;

int encryption(int m, int n);
int decryption(int c, int p, int q);
int mod(int k, int b, int m);
int modulo(int a, int b);
vector<int> extendedEuclid(int a, int b);


/* private key pair(p,q) of the form 3 mod 4 */
int p , q ;

/* public key n */
int n ;
int generate_random_number(int lower_bound, int upper_bound) {
    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the distribution
    std::uniform_int_distribution<int> distribution(lower_bound, upper_bound);

    // Generate and return a random number
    return distribution(gen);
}
bool is_prime(int num) {
    if (num < 2) {
        return false;
    }
    for (int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}
int numberOfDigits(int a) {
    int result = 0;
    while (a != 0) {
        a /= 10;
        result++;
    }
    return result;
}
bool same_number_ofDigits(int a, int b) {
    return numberOfDigits(a) == numberOfDigits(b);
}

std::pair<int, int> find_prime_pair(int n) {
    if (n < 2 || n > 999 * 999) {
        return std::make_pair(0, 0);
    }

    for (int i = 100; i <= 999; ++i) {
        if (is_prime(i) && is_prime(n / i) && i * (n / i) == n ) {
            if(i%4==3 && (n/i)%4==3 )
                if(numberOfDigits(i)==numberOfDigits(n/i)-1 || numberOfDigits(i)-1 == numberOfDigits(n / i))
            return std::make_pair(i, n / i);
        }
    }

    return std::make_pair(0, 0);
}



void initialize() {
    bool ok = false;
    while (!ok) {
        n = generate_random_number(10000, 99999);
        if (find_prime_pair(n).first != 0 && find_prime_pair(n).second != 0) {
            p = find_prime_pair(n).first;
            q = find_prime_pair(n).second;
            ok = true;
            cout << endl << endl;
            cout << "n=" << n << '\n';
            cout <<"p=" << p << " and  q=" << q << '\n\n\n';
            cout << endl;
        }
    }
}

int main()
{
    int i = 0;
    while (i < 10) {
        //char yes;

        initialize();
        /* vector for storing the encrypted message */
        vector<int>e;

        /* vector for storing the decrypted message */
        vector<int>d;

        string message = "Din fericire am invins";
        cout << "Plain text: " << message << endl;
        int len = strlen(message.c_str());
        //cout << "5929 9409 12996 11025 9409 1024 13456 10201 12996 10201 14884 9409 0" << endl;

        cout << "Encrypted text: ";
        for (int i = 0; i <= len; i++)
        {
            e.push_back(encryption(message[i], n));
            cout << e[i] << ' ';
        }
        cout << endl;
        for (int i = 0; i < len; i++)
        {
            d.push_back(decryption(e[i], p, q));
        }
        cout << "Decrypted text: ";
        for (int i = 0; i < d.size(); i++) {
            cout << char(d[i]);
            //cout << d[i];
        }
            //yes.append(char(d[i]));
        
        cout << endl;
        i++;
    }
    return 0;
}

int encryption(int m, int n)
{
    /* c = (m^2) mod n */
    int c = (m * m) % n;
    return c;
}

/* chinese remainder theorem implementation */
int mod(int k, int b, int m)
{
    int i = 0;
    int a = 1;
    vector<int> v;
    while (k > 0) {
        v.push_back(k % 2);
        k = (k - v[i]) / 2;
        i++;
    }
    for (int j = 0; j < i; j++) {
        if (v[j] == 1) {
            a = (a * b) % m;
            b = (b * b) % m;
        }
        else {
            b = (b * b) % m;
        }
    }
    return a;
}

int modulo(int a, int b)
{
    return a >= 0 ? a % b : (b - abs(a % b)) % b;
}

/* Extended Eucledian Algorithm */
vector<int> extendedEuclid(int a, int b)
{
    if (b > a) {
        int temp = a; a = b; b = temp;
    }
    int i = 0;
    int j = 1;
    int x = 1;
    int y = 0;
    while (b != 0) {
        int q = a / b;
        int temp1 = a % b;
        a = b;
        b = temp1;
        int temp2 = i;
        i = x - q * i;
        x = temp2;
        int temp3 = j;
        j = y - q * j;
        y = temp3;
    }
    vector<int>arr(3);
    arr[0] = x;
    arr[1] = y;
    arr[2] = 1;
    return arr;
}

int decryption(int c, int p, int q)
{
    int r = mod((p + 1) / 4, c, p);
    int s = mod((q + 1) / 4, c, q);

    vector<int> arr = extendedEuclid(p, q);
    int rootp = arr[0] * p * s;
    int rootq = arr[1] * q * r;
    double r1 = modulo((rootp + rootq), n);
    if (r1 < 128)
        return r1;
    int negative_r = n - r1;
    if (negative_r < 128)
        return negative_r;
    int s1 = modulo((rootp - rootq), n);
    if (s1 < 128)
        return s1;
    int negative_s = n - s1;
    return negative_s;
}