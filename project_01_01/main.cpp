#include <iostream>
#include <fstream>
#include <bitset>
#include <random>
#include <thread>
#include <chrono>
#include <string>

using namespace std;

namespace bit_math{
    bitset<1024> Addition(bitset<1024> a, bitset<1024> b){
        bitset<1024> carry = a & b;
        bitset<1024> result = a ^ b;
        while(carry != 0){
            bitset<1024> temp = carry << 1;
            carry = result & temp;
            result = result ^ temp;
        }
        return result;
    }

    bitset<1024> Multiple(bitset<1024> a, bitset<1024> b){
        bitset<1024> result("0");
        while(b.none() != 1){       
            if (b.test(0) & 1){
                result = bit_math::Addition(result, a);
            }
            a = a << 1;
            b = b >> 1;
        }
        return result;
    }

    int Find_MSB(bitset<1024> a){
        if(a.none() == 1){
            return 0;
        }
        int i = a.size() - 1;
        while(i != 0){
            if(a.test(i) == 1){
                return i;
            }
            i--;
        }
        return 0;
    }

    bool isLessEqual(bitset<1024> a, bitset<1024> b){
        bitset<1024> temp = a ^ b;
        int MSB = bit_math::Find_MSB(temp);
        if ((b.test(MSB)) == 1 || (MSB == 0 && a.test(MSB) == b.test(MSB))){
            return true;
        }
        return false;
    }

    bool isLess(bitset<1024> a, bitset<1024> b){
        bitset<1024> temp = a ^ b;
        int MSB = bit_math::Find_MSB(temp);
        if (temp.test(MSB) & b.test(MSB) == 1){
            return true;
        }
        return false;
    }

    bitset<1024> Square_root(bitset<1024> a){
        int msb = bit_math::Find_MSB(a);
        bitset<1024> temp = 1 << msb;
        bitset<1024> result("0");
        while(temp.none() != 1){
            bitset<1024> sum = bit_math::Addition(result, temp);
            if (isLessEqual(bit_math::Multiple(sum, sum), a)){
                result = bit_math::Addition(result, temp);
            }
            temp = temp >> 1;
        }
        return result;
    }

    bitset<1024> Subtract(bitset<1024> a, bitset<1024> b){
        while(b.none() != 1){
            bitset<1024> borrow = (~a) & b;
            a = a ^ b;
            b = borrow << 1;
        }
        return a;
    }

    bitset<1024> Division(bitset<1024> a, bitset<1024> b){
        bitset<1024> Ans("0");
        int size_a = Find_MSB(a);
        int size_b = Find_MSB(b) + 1;
        bitset<1024> temp;
        for (int i = size_a; i >= 0; i--){
            
            temp[0] = a[i];
            if (isLess(temp, b)){
                temp = temp << 1;
                Ans = Ans << 1;
            }
            else{
                temp = Subtract(temp, b);
                temp = temp << 1;
                Ans[0] = 1;
                Ans = Ans << 1;
            }
        }
        Ans = Ans >> 1;
        return Ans;
    }

    bitset<1024> Modulo(bitset<1024> a, bitset<1024> b){
        return Subtract(a, Multiple(b, Division(a, b)));
    }

    bitset<1024> Power(bitset<1024> a, bitset<1024> b, bitset<1024> p){
        bitset<1024> result(1);
        a = Modulo(a, p);
        while (b.none() != 1){
            if (b.test(0) & 1){
                result = Modulo(Multiple(result, a), p);
            }
            b = b >> 1;
            a = Modulo(Multiple(a, a), p);
        }
        return result;
    }
}

void reverStr(string& str){
    int n = str.length();
    for (int i = 0; i < n / 2; i++){
        swap(str[i], str[n - i - 1]);
    }
}

void Read_File(string* Text, const char* File){
    fstream MyFile;
    MyFile.open(File);
    getline(MyFile, *Text);
    MyFile.close();
    return;
}

void Write_File(string Text, const char* File){
    ofstream MyFile;
    MyFile.open(File);
    MyFile << Text;
    MyFile.close();
    return;
}

string Hex_to_Bytes(char c){
    switch(toupper(c))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
    }
    return NULL;
}

void Num_to_Bit(string& Num){
    string newNum;
    int n = Num.length();
    for (int i = 0; i < n; i++){
        string temp = Hex_to_Bytes(Num[i]);
        newNum.append(temp);
    }
    Num = newNum;
}

bitset<1024> random_bitset(int MSB, double p = 0.5) {
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution d(0.5);
    bitset<1024> t;
    for (int i = 0; i < MSB; i++){
        t[i] = d(gen);
    }
    return t;
}

bool MillerTest(bitset<1024> d, bitset<1024> n){
    bitset<1024> a = bit_math::Addition(bit_math::Modulo(random_bitset(bit_math::Find_MSB(n), 0.5), bit_math::Subtract(n, 4)), 2);
    bitset<1024> x = bit_math::Power(a, d, n);
    if (x == 1 || x == bit_math::Subtract(n, 1)){
        return true;
    }

    while (d != bit_math::Subtract(n, 1)){
        x = bit_math::Modulo(bit_math::Multiple(x, x), n);
        d = d << 1;
        if (x == 1)return false;
        if (x == bit_math::Subtract(n, 1))return true;
    }
    return false;
}

bool isPrime(bitset<1024> n, int k){
    if (n[0] == 0)return false;
    if (bit_math::isLessEqual(n, 1) || n == (bitset<1024>)4) return false;
    if (bit_math::isLessEqual(n,3)) return true;
    
    bitset<1024> d = bit_math::Subtract(n, 1);
    while (d[0] == 0){
        d = d >> 1;
    }
    for (int i = 0; i < k; i++){
        if (!MillerTest(d, n))
            return false;
    }
    return true;
}

void Task(char** argv){
    string myText;
    Read_File(&myText, argv[1]);
    reverStr(myText);
    Num_to_Bit(myText);
    bool flag = true;
    bitset<1024> Num(myText);
    bitset<1024> n(31);
    bitset<1024> temp;
    bitset<1024> temp_2;
    bool result =  isPrime(Num, 8);
    Write_File(to_string(result), argv[2]);
    exit(0);
}

int main(int argc, char **argv){
    const int time = 60;
    thread n_thresh(Task, argv);
    this_thread::sleep_for(chrono::seconds(time));
    return 0;
}
